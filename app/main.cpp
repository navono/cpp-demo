// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <fmt/format.h>
#include <folly/FBString.h>

#include <fstream>

#include "config.h"
#include "ctrl-c.h"
#include "example.h"
#include "logger.h"

using namespace std;

// Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int addAddressBook(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
    return -1;
  }

  tutorial::AddressBook address_book;

  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
    } else if (!address_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
  }

  // Add an address.
  Dummy::PromptForAddress(address_book.add_people());

  {
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
      cerr << "Failed to write address book." << endl;
      return -1;
    }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();
}

int main(int argc, char* argv[]) {
  auto logger = initLogger();

  LOG_INFO(logger, "C++ Demo v{}.{}.{}.{}", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH,
           PROJECT_VERSION_TWEAK);

  folly::fbstring fs("World");
  LOG_INFO(logger, "Hello: {}!", fs.toStdString());

  std::mutex wait_lock;
  std::condition_variable wait_var;
  const unsigned int kMaxCatches = 2;
  unsigned int catches = 0;

  unsigned int handler_id = CtrlCLibrary::SetCtrlCHandler(
      [&logger, &catches, &wait_lock, &wait_var](enum CtrlCLibrary::CtrlSignal event) -> bool {
        switch (event) {
          case CtrlCLibrary::kCtrlCSignal:
            LOG_INFO(logger, "Catch Ctrl+C");
        }
        std::lock_guard<std::mutex> locker(wait_lock);
        ++catches;
        wait_var.notify_all();
        return true;
      });
  if (handler_id == CtrlCLibrary::kErrorID) {
    LOG_ERROR(logger, "Can't set ctrl+c handler");
    return 0;
  }

  LOG_INFO(logger, "Press Ctrl+C {} times", kMaxCatches);

  //  addAddressBook(argc, argv);

  tutorial::Person person;
  person.set_email("123@123.com");
  person.set_id(2);
  auto s = person.SerializeAsString();

  std::unique_lock<std::mutex> locker(wait_lock);
  wait_var.wait(locker, [&catches, kMaxCatches]() { return catches >= kMaxCatches; });
  CtrlCLibrary::ResetCtrlCHandler(handler_id);

  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  //  Dummy d = Dummy();
  return Dummy::doSomething() ? 0 : -1;
}
