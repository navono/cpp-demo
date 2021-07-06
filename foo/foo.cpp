#include "foo.h"

#include <folly/executors/CPUThreadPoolExecutor.h>

#include <iostream>

#include "interface.h"
#include "logger.h"

using namespace std;

foo::foo(std::shared_ptr<zmq::context_t> ctx, const std::string& subAddr) : ctx_(ctx), subAddr_(subAddr) {
  logger_ = initLogger("foo");
};

foo::~foo() = default;

void foo::hello() {
  LOG_INFO(logger_, "hello from foo.dll");

  auto executor = std::make_unique<folly::CPUThreadPoolExecutor>(4);
  folly::makeFuture(true).via(executor.get()).thenValue([&](bool b) { xPublisher(); });
}

void foo::stop() { exitSignal_.set_value(); }

void foo::subscriberThread() {
  auto zmqCtx = std::make_shared<zmq::context_t>(1);
  zmq::socket_t subscriber(*zmqCtx, zmq::socket_type::sub);
  subscriber.connect("tcp://localhost:5555");

  //  Prepare subscriber
  //  zmq::socket_t subscriber(*ctx_, zmq::socket_type::sub);
  //  subscriber.connect("inproc://#1");

  //  Thread2 opens "A" and "B" envelopes
  subscriber.set(zmq::sockopt::subscribe, "A");
  subscriber.set(zmq::sockopt::subscribe, "B");

  auto fut = getExitFut();
  while (fut.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout) {
    // Receive all parts of the message
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
    assert(result && "recv failed");
    assert(*result == 2);

    LOG_INFO(logger_, "receive msg. topic: {}, msg: {}", recv_msgs[0].to_string(), recv_msgs[1].to_string());
  }

  //  size_t nmsg = 10;
  //  size_t nrx = 0;
  //
  //  // to use zmq_poll correctly, we construct this vector of pollitems
  //  std::vector<zmq::pollitem_t> p = {{subscriber, 0, ZMQ_POLLIN, 0}};
  //  while (true) {
  //    zmq::message_t rx_msg;
  //    // when timeout (the third argument here) is -1,
  //    // then block until ready to receive
  //    zmq::poll(p.data(), 1, -1);
  //    if (p[0].revents & ZMQ_POLLIN) {
  //      // received something on the first (only) socket
  //      subscriber.recv(&rx_msg);
  //      std::string rx_str;
  //      rx_str.assign(static_cast<char*>(rx_msg.data()), rx_msg.size());
  //      std::cout << "Received: " << rx_str << std::endl;
  //      if (++nrx == nmsg) break;
  //    }
  //  }

  LOG_INFO(logger_, "exit subscriber thread");
}

void foo::xPublisher() {
  auto ctx = std::make_shared<zmq::context_t>(1);

  zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
  publisher.connect(subAddr_);

  auto fut = getExitFut();
  while (fut.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout) {
    publisher.send(zmq::str_buffer("A"), zmq::send_flags::sndmore);
    publisher.send(zmq::str_buffer("Message in A envelope from foo"));
    publisher.send(zmq::str_buffer("B"), zmq::send_flags::sndmore);
    publisher.send(zmq::str_buffer("Message in B envelope from foo"));
    publisher.send(zmq::str_buffer("C"), zmq::send_flags::sndmore);
    publisher.send(zmq::str_buffer("Message in C envelope from foo"));

    std::this_thread::sleep_for(std::chrono::seconds(1));
    LOG_INFO(logger_, "send message to TOPIC A, B, C");
  }
}

std::future<void> foo::getExitFut() { return exitSignal_.get_future(); }

// std::unique_ptr<folly::DMPSCQueue<int, false>> foo::get_queue() {
//  //  folly::DMPSCQueue<int, false> queue(1000);
//  return queue_;
//}

IModule* DYNALO_CALL CreateFoo(std::shared_ptr<zmq::context_t> ctx, const std::string& subAddr) {
  return new foo(ctx, subAddr);
}
