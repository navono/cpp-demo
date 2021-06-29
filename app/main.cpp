// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <fmt/format.h>
#include <folly/FBString.h>
#include <folly/Format.h>
#include <folly/Uri.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>

#include <iostream>

#include "config.h"
#include "ctrl-c.h"
#include "example.h"
#include "logger.h"

static void print_uri(const folly::fbstring& address, folly::Future<bool>& exit_future) {
  while (true) {
    try {
      const folly::Uri uri(address);
      const auto authority = folly::format("The authority from {} is {}", uri.fbstr(), uri.authority());
      LOG_INFO(quill::get_logger("app"), "{}", authority.str());

      exit_future.wait(std::chrono::milliseconds(2000));

      if (exit_future.hasValue()) {
        LOG_INFO(quill::get_logger("app"), "exit_future has value");
        return;
      }
    } catch (const std::exception& e) {
      LOG_ERROR(quill::get_logger("app"), "exit_future has error {}", e.what());
    }
  }
}

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

  //  folly::ThreadedExecutor executor;
  //  folly::Promise<folly::fbstring> promise;
  //
  //  folly::Future<folly::fbstring> future = promise.getSemiFuture().via(&executor);
  //  folly::Future<folly::Unit> unit = std::move(future).thenValue(print_uri);

  //  promise.setValue("https://conan.io/");
  //  std::move(unit).get();

  folly::Promise<bool> exit_promise;                           // create promise object
  folly::Future<bool> exit_future = exit_promise.getFuture();  // create future objects

  folly::CPUThreadPoolExecutor executor(5);
  folly::Promise<folly::fbstring> promise;

  folly::Future<folly::fbstring> fut = promise.getSemiFuture().via(&executor);
  //  auto unit = std::move(fut).thenValue(print_uri);

  //  auto unit = std::move(fut).thenValue([=, &exit_future](folly::fbstring&& v) { print_uri(v, exit_future); });
  std::move(fut).thenValue([=, &exit_future](folly::fbstring&& v) { print_uri(v, exit_future); });

  promise.setValue("https://conan.io/");
  std::this_thread::sleep_for(std::chrono::seconds(7));  // wait for 7 seconds

  exit_promise.setValue(true);
  //  std::move(unit).get();

  // 阻塞跟 executor 的执行函数，直到其退出
  executor.stop();

  std::unique_lock<std::mutex> locker(wait_lock);
  wait_var.wait(locker, [&catches, kMaxCatches]() { return catches >= kMaxCatches; });
  CtrlCLibrary::ResetCtrlCHandler(handler_id);

  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  //  Dummy d = Dummy();
  return Dummy::doSomething() ? 0 : -1;
}
