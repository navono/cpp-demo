#pragma once

#include <folly/futures/Future.h>
#include <quill/Quill.h>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "dynalo/symbol_helper.hpp"
#include "interface.h"

class foo : public IModule {
 public:
  foo(std::shared_ptr<zmq::context_t> ctx);
  ~foo() override;

  void hello() override;
  folly::SemiFuture<int> get_fut() override;
  //  std::unique_ptr<folly::DMPSCQueue<int, false>> get_queue() override;
  bool set_queue(folly::DMPSCQueue<int, false>& queue) override;

 private:
  void subscriberThread1();

 private:
  std::shared_ptr<quill::Logger> logger_;
  folly::Promise<int> promise_;

  std::shared_ptr<zmq::context_t> ctx_;
};
