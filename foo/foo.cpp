#include "foo.h"

#include <folly/executors/CPUThreadPoolExecutor.h>

#include <iostream>

#include "interface.h"
#include "logger.h"

using namespace std;

foo::foo(std::shared_ptr<zmq::context_t> ctx) : ctx_(ctx) { logger_ = initLogger("foo"); };

foo::~foo() = default;

void foo::hello() {
  LOG_INFO(logger_, "hello from foo.dll");

  //  folly::CPUThreadPoolExecutor executor{4};
  //  auto fut = folly::makeFuture(true);
  //  auto exeFut = std::move(fut).via(&executor);
  //  std::move(exeFut).thenValue([&](bool b) { subscriberThread1(); });

  auto executor = std::make_unique<folly::CPUThreadPoolExecutor>(4);
  auto fut = folly::makeFuture(true);
  auto exeFut = std::move(fut).via(executor.get());
  std::move(exeFut).thenValue([&](bool b) { subscriberThread1(); });

  //  auto fut = folly::makeFuture(true);
  //  auto exeFut = std::move(fut).via(executor.get());
  //  std::move(exeFut).thenValue([&](bool b) { publisherThread(zmqCtx.get()); });
}

folly::SemiFuture<int> foo::get_fut() {
  promise_.setValue(100);
  return promise_.getSemiFuture();
}
bool foo::set_queue(folly::DMPSCQueue<int, false>& queue) {
  //  queue_ = std::move(queue);
  //  queue.set_queue(queue_);
  //  set_queue(queue)
  //  queue_(std::move(queue));

  queue.try_enqueue(200);
  return true;
}

void foo::subscriberThread1() {
  //  Prepare subscriber
  zmq::socket_t subscriber(*ctx_, zmq::socket_type::sub);
  subscriber.connect("inproc://#1");

  //  Thread2 opens "A" and "B" envelopes
  subscriber.set(zmq::sockopt::subscribe, "A");
  subscriber.set(zmq::sockopt::subscribe, "B");

  while (1) {
    // Receive all parts of the message
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
    assert(result && "recv failed");
    assert(*result == 2);

    LOG_INFO(logger_, "receive msg. topic: {}, msg: {}", recv_msgs[0].to_string(), recv_msgs[1].to_string());
  }
}

// std::unique_ptr<folly::DMPSCQueue<int, false>> foo::get_queue() {
//  //  folly::DMPSCQueue<int, false> queue(1000);
//  return queue_;
//}

IModule* DYNALO_CALL CreateFoo(std::shared_ptr<zmq::context_t> ctx) { return new foo(ctx); }
