#include <cppzmq/zmq.hpp>
#include <cppzmq/zmq_addon.hpp>
#include <thread>
#include <future>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

void PublisherThread(zmq::context_t *ctx, const std::string &addr);
void SubscriberThread2(zmq::context_t *ctx, const std::string &addr);
void SubscriberThread3(zmq::context_t *ctx, const std::string &addr);

#ifdef _WIN32
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

#else
int main(int argc, char **argv) {
  Config::initializeArgs(argc, argv);
  //  NUIKIT_DEBUG(fmt::format("optDaemon: {}, optDebug: {}", Config::optDaemon, Config::optDebug));

#endif

  /*
   * No I/O threads are involved in passing messages using the inproc transport.
   * Therefore, if you are using a ØMQ context for in-process messaging only you
   * can initialise the context with zero I/O threads.
   *
   * Source: http://api.zeromq.org/4-3:zmq-inproc
   */
  zmq::context_t ctx(0);
  auto addr = "inproc://#1";

  auto thread1 = std::async(std::launch::async, PublisherThread, &ctx, addr);

  // Give the publisher a chance to bind, since inproc requires it
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  auto thread2 = std::async(std::launch::async, SubscriberThread2, &ctx, addr);
  //  auto thread3 = std::async(std::launch::async, SubscriberThread3, &ctx, addr);
  thread1.wait();
  thread2.wait();
  //  thread3.wait();

  std::cout << "Hello, World!" << std::endl;
  return 0;
}

void PublisherThread(zmq::context_t *ctx, const std::string &addr) {
  //  Prepare publisher
  zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
  publisher.bind(addr);

  // Give the subscribers a chance to connect, so they don't lose any messages
  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  while (true) {
    //  Write three messages, each with an envelope and content
    publisher.send(zmq::str_buffer("A"), zmq::send_flags::sndmore);
    publisher.send(zmq::str_buffer("Message in A envelope"));
    publisher.send(zmq::str_buffer("B"), zmq::send_flags::sndmore);
    publisher.send(zmq::str_buffer("Message in B envelope"));
    publisher.send(zmq::str_buffer("C"), zmq::send_flags::sndmore);
    publisher.send(zmq::str_buffer("Message in C envelope"));
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
}

void SubscriberThread2(zmq::context_t *ctx, const std::string &addr) {
  //  Prepare subscriber
  zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
  subscriber.connect(addr);

  //  Thread2 opens "A" and "B" envelopes
  subscriber.set(zmq::sockopt::subscribe, "A");
  subscriber.set(zmq::sockopt::subscribe, "B");

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // Receive all parts of the message
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
    assert(result && "recv failed");
    assert(*result == 2);

    std::cout << "Thread2: [" << recv_msgs[0].to_string() << "] " << recv_msgs[1].to_string() << std::endl;
  }
}

void SubscriberThread3(zmq::context_t *ctx, const std::string &addr) {
  //  Prepare our context and subscriber
  zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
  subscriber.connect(addr);

  //  Thread3 opens ALL envelopes
  subscriber.set(zmq::sockopt::subscribe, "");

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // Receive all parts of the message
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
    assert(result && "recv failed");
    assert(*result == 2);

    std::cout << "Thread3: [" << recv_msgs[0].to_string() << "] " << recv_msgs[1].to_string() << std::endl;
  }
}