// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <fmt/format.h>
#include <folly/FBString.h>
#include <folly/executors/ThreadedExecutor.h>

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

  dynalo::library lib("./foo.dll");
  auto pfnCreateFoo = lib.get_function<IModule*()>("CreateFoo");

  folly::DMPSCQueue<int, false> queue(100);
  if (pfnCreateFoo) {
    auto f = pfnCreateFoo();
    f->hello();
    auto fut = f->get_fut();
    folly::ThreadedExecutor executor;
    auto fut2 = std::move(fut).via(&executor);
    std::move(fut2).thenValue([&](int i) { LOG_INFO(logger, "val: {}", i); });

    f->set_queue(queue);
    int v;
    queue.try_dequeue(v);
    LOG_INFO(logger, "v {}", v);
  }

  std::unique_lock<std::mutex> locker(wait_lock);
  wait_var.wait(locker, [&catches, kMaxCatches]() { return catches >= kMaxCatches; });
  CtrlCLibrary::ResetCtrlCHandler(handler_id);

  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  //  Dummy d = Dummy();
  return Dummy::doSomething() ? 0 : -1;
}
