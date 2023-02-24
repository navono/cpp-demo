#include "sharedLib/lib.h"

#include <iostream>

namespace lib {
[[noreturn]] void Hello::sayHello(zmq::context_t *ctx, const std::string &addr) {
  std::cout << "Hello Shared Library!" << std::endl;

  //  Prepare our context and subscriber
  zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
  subscriber.connect(addr);

  //  Thread3 opens ALL envelopes
  subscriber.set(zmq::sockopt::subscribe, "");

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // Receive all parts of the message
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
    assert(result && "recv failed");
    assert(*result == 2);

    std::cout << "SharedLib: [" << recv_msgs[0].to_string() << "] " << recv_msgs[1].to_string() << std::endl;
  }
}
}  // namespace lib
