#include <folly/FBString.h>
#include <folly/Format.h>
#include <folly/Uri.h>
#include <folly/concurrency/DynamicBoundedQueue.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>
#include <quill/Quill.h>

#include "example.h"

static void print_uri(quill::Logger* logger, const folly::fbstring& address, folly::Future<bool>& exit_future) {
  while (true) {
    try {
      const folly::Uri uri(address);
      const auto authority = folly::format("The authority from {} is {}", uri.fbstr(), uri.authority());
      LOG_INFO(logger, "{}", authority.str());

      exit_future.wait(std::chrono::milliseconds(2000));

      if (exit_future.hasValue()) {
        LOG_INFO(logger, "exit_future has value");
        return;
      }
    } catch (const std::exception& e) {
      LOG_ERROR(logger, "exit_future has error {}", e.what());
    }
  }
}

folly::SemiFuture<std::string> get_fut() {
  folly::Promise<std::string> p;
  p.setValue("hello");
  return p.getSemiFuture();
}

void basic_test(quill::Logger* logger) {
  folly::ThreadedExecutor executor;
  //  LOG_INFO(logger, "making Promise");
  //  folly::Promise<int> p;
  //  folly::Future<int> f = p.getSemiFuture().via(&executor);
  //
  //  auto f2 = std::move(f).thenValue([&logger](int x) { LOG_INFO(logger, "{}", x); });
  //
  //  LOG_INFO(logger, "fulfilling Promise");
  //  p.setValue(42);
  //
  //  std::move(f2).wait();
  //  LOG_INFO(logger, "Promise fulfilled");

  folly::SemiFuture<std::string> fut = get_fut();
  auto fut2 = std::move(fut).via(&executor);
  std::move(fut2).thenValue([&logger](std::string x) { LOG_INFO(logger, "{}", x); });
  std::move(fut2).wait();
  LOG_INFO(logger, "Promise fulfilled");
}

void executor_test(quill::Logger* logger) {
  folly::Promise<bool> exit_promise;                           // create promise object
  folly::Future<bool> exit_future = exit_promise.getFuture();  // create future objects

  folly::CPUThreadPoolExecutor executor(5);
  folly::Promise<folly::fbstring> promise;

  folly::Future<folly::fbstring> fut = promise.getSemiFuture().via(&executor);
  //  auto unit = std::move(fut).thenValue(print_uri);

  //  auto unit = std::move(fut).thenValue([=, &exit_future](folly::fbstring&& v) { print_uri(v, exit_future);
  std::move(fut).thenValue([=, &exit_future](folly::fbstring&& v) { print_uri(logger, v, exit_future); });

  promise.setValue("https://conan.io/");
  std::this_thread::sleep_for(std::chrono::seconds(7));  // wait for 7 seconds

  exit_promise.setValue(true);
  //  std::move(unit).get();

  // 阻塞跟 executor 的执行函数，直到其退出
  executor.stop();
}

void dynamic_queue_test(quill::Logger* logger) {
  folly::DMPSCQueue<int, false, 10> queue(1000);

  auto deadline = std::chrono::seconds(2);

  std::thread reader([&queue, &logger, &deadline] {
    for (;;) {
      int val;
      if (queue.try_dequeue_for(val, deadline)) {
        LOG_INFO(logger, "get value {}", val);
      }
    }
  });

  for (;;) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    queue.try_enqueue(3);
  }
}

void t(quill::Logger* logger) {}

void folly_test() {
  auto logger = quill::get_logger("app");

  basic_test(logger);
  //  executor_test(logger);
  //  dynamic_queue_test(logger);
}
