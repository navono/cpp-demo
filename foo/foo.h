#pragma once

#include <folly/futures/Future.h>
#include <quill/Quill.h>

#include <future>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "dynalo/symbol_helper.hpp"
#include "interface.h"

class foo : public IModule {
 public:
  explicit foo(std::shared_ptr<zmq::context_t> ctx, const std::string& subAddr);
  ~foo() override;

  void hello() override;
  void stop() override;

 private:
  void subscriberThread();

  void xPublisher();

  std::future<void> getExitFut();

 private:
  std::shared_ptr<quill::Logger> logger_;
  std::promise<void> exitSignal_;

  std::shared_ptr<zmq::context_t> ctx_;
  std::string subAddr_;
};
