#include <iostream>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "example.h"

void publisherThread(const std::shared_ptr<quill::Logger>& logger, zmq::context_t* ctx) {
  //  Prepare publisher
  zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
  //  publisher.bind("inproc://#1");
  publisher.bind("tcp://*:5555");

  // Give the subscribers a chance to connect, so they don't lose any messages
  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  while (true) {
    try {
      //  Write three messages, each with an envelope and content
      publisher.send(zmq::str_buffer("A"), zmq::send_flags::sndmore);
      publisher.send(zmq::str_buffer("Message in A envelope"));
      publisher.send(zmq::str_buffer("B"), zmq::send_flags::sndmore);
      publisher.send(zmq::str_buffer("Message in B envelope"));
      publisher.send(zmq::str_buffer("C"), zmq::send_flags::sndmore);
      publisher.send(zmq::str_buffer("Message in C envelope"));
      std::this_thread::sleep_for(std::chrono::seconds(1));
      //      LOG_INFO(logger, "send message to TOPIC A, B, C");
    } catch (const std::exception& e) {
      LOG_ERROR(logger, "catch exception {}", e.what());
    }
  }
}

void xPubSub(const std::shared_ptr<quill::Logger>& logger) {
  auto ctx = std::make_shared<zmq::context_t>(1);

  // Incoming publications come here
  //  zmq::socket_t sub(*ctx, ZMQ_PULL);
  zmq::socket_t sub(*ctx, zmq::socket_type::pull);
  sub.bind("tcp://127.0.0.1:5555");

  // Outgoing publications go out through here.
  //  zmq::socket_t pub(*ctx, ZMQ_PUB);
  zmq::socket_t pub(*ctx, zmq::socket_type::pub);
  pub.bind("tcp://127.0.0.1:5556");

  //  zmq::proxy(sub, pub, nullptr);

  zmq::pollitem_t poll_items[]{{sub, 0, ZMQ_POLLIN, 0}, {pub, 0, ZMQ_POLLIN, 0}};

  while (true) {
    assert(zmq::poll(poll_items, 2));
  }

  //  zmq::poller_t<> in_poller, out_poller;
  //  in_poller.add(pub, zmq::event_flags::pollin);
  //  out_poller.add(sub, zmq::event_flags::pollin);
  //
  //  const std::chrono::milliseconds timeout{100};
  //  std::vector<zmq::poller_event<>> in_events(1);
  //  std::vector<zmq::poller_event<>> out_events(1);
  //  while (true) {
  //    const auto nin = in_poller.wait_all(in_events, timeout);
  //    if (!nin) {
  //      std::cout << "input timeout, try again" << std::endl;
  //      continue;
  //    }
  //    for (int ind = 0; ind < nin; ++ind) {
  //      //      zmq::message_t msg;
  //      //      auto rres = in_events[ind].socket.recv(msg, zmq::recv_flags::none);
  //
  //      std::vector<zmq::message_t> recv_msgs;
  //      zmq::recv_result_t result = zmq::recv_multipart(in_events[ind].socket, std::back_inserter(recv_msgs));
  //
  //      const auto nout = out_poller.wait_all(out_events, timeout);
  //      if (!nout) {
  //        std::cout << "output timeout, freakout" << std::endl;
  //        abort();
  //      }
  //      out_events[0].socket.send(recv_msgs[0], zmq::send_flags::sndmore);
  //      out_events[0].socket.send(recv_msgs[1], zmq::send_flags::none);
  //    }
  //  }
}

void xPubSubProxy(const std::string& xPub_addr, const std::string& xSub_addr,
                  const std::shared_ptr<quill::Logger>& logger, const std::shared_ptr<zmq::context_t>& subCtx) {
  auto ctx = std::make_shared<zmq::context_t>(1);

  // 处理 Publishers
  zmq::socket_t xSub_socket(*ctx, zmq::socket_type::xsub);
  xSub_socket.bind(xSub_addr);

  // 处理 Subscribers
  zmq::socket_t xPub_socket(*subCtx, zmq::socket_type::xpub);
  xPub_socket.bind(xPub_addr);

  // 3.初始化轮询集合
  zmq_pollitem_t items[] = {{xSub_socket, 0, ZMQ_POLLIN, 0}, {xPub_socket, 0, ZMQ_POLLIN, 0}};

  while (true) {
    zmq::message_t msg;

    // 调用zmq_poll轮询消息
    auto rc = zmq::poll(items, 2, -1);
    if (rc == -1) {
      // error
      LOG_ERROR(logger, "poll error");
      return;
    } else if (rc == 0) {
      // 超时
      LOG_WARNING(logger, "poll timeout");
      continue;
    } else {
      std::vector<zmq::message_t> recv_msgs;

      if (items[0].revents & ZMQ_POLLIN) {
        // 数据是 Publisher 发送过来的
        zmq::recv_result_t result = zmq::recv_multipart(xSub_socket, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        LOG_INFO(logger, "xSub_socket receive messages, send it to xSub_socket");

        //        xSub_socket.send(recv_msgs[0], zmq::send_flags::sndmore);
        //        xSub_socket.send(recv_msgs[1], zmq::send_flags::none);
        zmq::send_multipart(xPub_socket, recv_msgs);
      } else if (items[1].revents & ZMQ_POLLIN) {
        zmq::recv_result_t result = zmq::recv_multipart(xPub_socket, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        LOG_INFO(logger, "xPub_socket receive messages, send it to xPub_socket");
        zmq::send_multipart(xSub_socket, recv_msgs);
      }
    }
  }
}

void xSubscriber(const std::string& xPub_addr, const std::shared_ptr<quill::Logger>& logger,
                 const std::shared_ptr<zmq::context_t>& subCtx) {
  //  auto ctx = std::make_shared<zmq::context_t>(1);

  zmq::socket_t sub(*subCtx, zmq::socket_type::sub);
  sub.connect(xPub_addr);

  sub.set(zmq::sockopt::subscribe, "");

  int update_nbr;
  for (update_nbr = 0; update_nbr < 15; ++update_nbr) {
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(sub, std::back_inserter(recv_msgs));
    assert(result && "recv failed");
    assert(*result == 2);

    auto topic = recv_msgs[0].to_string();
    LOG_INFO(logger, "subscriber receive msg. topic: {}, msg: {}", recv_msgs[0].to_string(), recv_msgs[1].to_string());
  }

  LOG_INFO(logger, "subscriber exit");
  sub.close();
  //  ctx->close();
}
