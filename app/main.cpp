// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <fmt/format.h>
#include <folly/FBString.h>
#include <folly/executors/CPUThreadPoolExecutor.h>

#include <zmq.hpp>

#include "config.h"
#include "ctrl-c.h"
#include "dynalo/dynalo.hpp"
#include "example.h"
#include "interface.h"
#include "logger.h"

/*
 * Simple main program that demonstrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
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

  //  zmq::context_t ctx{0};
  auto zmqCtx = std::make_shared<zmq::context_t>(0);

  auto executor = std::make_unique<folly::CPUThreadPoolExecutor>(4);
  auto fut = folly::makeFuture(true);
  auto exeFut = std::move(fut).via(executor.get());
  std::move(exeFut).thenValue([&](bool b) { publisherThread(logger, zmqCtx.get()); });

  dynalo::library lib("./foo.dll");
  auto pfnCreateFoo = lib.get_function<IModule*(std::shared_ptr<zmq::context_t> ctx)>("CreateFoo");
  if (pfnCreateFoo) {
    auto f = pfnCreateFoo(zmqCtx);

    auto fut_f = folly::makeFuture(true);
    auto exeFut_f = std::move(fut_f).via(executor.get());
    std::move(exeFut_f).thenValue([&](bool b) { f->hello(); });

    std::this_thread::sleep_for(std::chrono::seconds(5));
    LOG_INFO(logger, "stop foo subscriber");
    //    f->stop();
  }

  LOG_INFO(logger, "Test");

  std::unique_lock<std::mutex> locker(wait_lock);
  wait_var.wait(locker, [&catches, kMaxCatches]() { return catches >= kMaxCatches; });
  CtrlCLibrary::ResetCtrlCHandler(handler_id);

  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  //  Dummy d = Dummy();
  return Dummy::doSomething() ? 0 : -1;
}
