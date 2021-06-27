// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <fmt/format.h>
#include <folly/FBString.h>
#include <spdlog/spdlog.h>

#include <iostream>

#include "config.h"
#include "ctrl-c.h"
#include "example.h"

/*
 * Simple main program that demonstrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
  std::cout << "C++ Demo v" << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "."
            << PROJECT_VERSION_PATCH << "." << PROJECT_VERSION_TWEAK << std::endl;

  folly::fbstring fs("World");

  // Use the default logger (stdout, multi-threaded, colored)
  spdlog::info(fmt::format("Hello: {}!", fs.toStdString()));

  std::mutex wait_lock;
  std::condition_variable wait_var;
  const unsigned int kMaxCatches = 2;
  unsigned int catches = 0;

  unsigned int handler_id =
      CtrlCLibrary::SetCtrlCHandler([&catches, &wait_lock, &wait_var](enum CtrlCLibrary::CtrlSignal event) -> bool {
        switch (event) {
          case CtrlCLibrary::kCtrlCSignal:
            spdlog::info("Catch Ctrl+C");
        }
        std::lock_guard<std::mutex> locker(wait_lock);
        ++catches;
        wait_var.notify_all();
        return true;
      });
  if (handler_id == CtrlCLibrary::kErrorID) {
    spdlog::error("Can't set ctrl+c handler");
    return 0;
  }

  spdlog::info(fmt::format("Press Ctrl+C {} times", kMaxCatches));

  std::unique_lock<std::mutex> locker(wait_lock);
  wait_var.wait(locker, [&catches, kMaxCatches]() { return catches >= kMaxCatches; });
  CtrlCLibrary::ResetCtrlCHandler(handler_id);

  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  //  Dummy d = Dummy();
  return Dummy::doSomething() ? 0 : -1;
}
