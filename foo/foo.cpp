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

// folly::SemiFuture<int> foo::get_fut() {
//  promise_.setValue(100);
//  return promise_.getSemiFuture();
//}

void foo::stop() { exitSignal_.set_value(); }

bool foo::set_queue(folly::DMPSCQueue<int, false>& queue) {
  //  queue_ = std::move(queue);
  //  queue.set_queue(queue_);
  //  set_queue(queue)
  //  queue_(std::move(queue));

  queue.try_enqueue(200);
  return true;
}

void foo::subscriberThread1() {
  //  auto zmqCtx = std::make_shared<zmq::context_t>(0);
  //  zmq::socket_t subscriber(*zmqCtx, zmq::socket_type::sub);

  //  Prepare subscriber
  zmq::socket_t subscriber(*ctx_, zmq::socket_type::sub);
  subscriber.connect("inproc://#1");
  //  subscriber.connect("tcp://localhost:5556");

  //  Thread2 opens "A" and "B" envelopes
  subscriber.set(zmq::sockopt::subscribe, "A");
  subscriber.set(zmq::sockopt::subscribe, "B");

  //  auto fut = getExitFut();
  //
  //  while (fut.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout) {
  //    // Receive all parts of the message
  //    std::vector<zmq::message_t> recv_msgs;
  //    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
  //    assert(result && "recv failed");
  //    assert(*result == 2);
  //
  //    LOG_INFO(logger_, "receive msg. topic: {}, msg: {}", recv_msgs[0].to_string(), recv_msgs[1].to_string());
  //  }

  size_t nmsg = 10;
  size_t nrx = 0;

  // to use zmq_poll correctly, we construct this vector of pollitems
  std::vector<zmq::pollitem_t> p = {{subscriber, 0, ZMQ_POLLIN, 0}};
  while (true) {
    zmq::message_t rx_msg;
    // when timeout (the third argument here) is -1,
    // then block until ready to receive
    zmq::poll(p.data(), 1, -1);
    if (p[0].revents & ZMQ_POLLIN) {
      // received something on the first (only) socket
      subscriber.recv(&rx_msg);
      std::string rx_str;
      rx_str.assign(static_cast<char*>(rx_msg.data()), rx_msg.size());
      std::cout << "Received: " << rx_str << std::endl;
      if (++nrx == nmsg) break;
    }
  }

  LOG_INFO(logger_, "exit subscriber thread");
}

std::future<void> foo::getExitFut() { return exitSignal_.get_future(); }

// std::unique_ptr<folly::DMPSCQueue<int, false>> foo::get_queue() {
//  //  folly::DMPSCQueue<int, false> queue(1000);
//  return queue_;
//}

IModule* DYNALO_CALL CreateFoo(std::shared_ptr<zmq::context_t> ctx) { return new foo(ctx); }
