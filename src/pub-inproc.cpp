#include <iostream>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

void publisherThread(zmq::context_t* ctx) {
  //  Prepare publisher
  zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
  publisher.bind("inproc://#1");

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
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
}

// void SubscriberThread2(zmq::context_t* ctx) {
//  //  Prepare our context and subscriber
//  zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
//  subscriber.connect("inproc://#1");
//
//  //  Thread3 opens ALL envelopes
//  subscriber.set(zmq::sockopt::subscribe, "");
//
//  while (1) {
//    // Receive all parts of the message
//    std::vector<zmq::message_t> recv_msgs;
//    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
//    assert(result && "recv failed");
//    assert(*result == 2);
//
//    std::cout << "Thread3: [" << recv_msgs[0].to_string() << "] " << recv_msgs[1].to_string() << std::endl;
//  }
//}