#pragma once

#include <folly/futures/Future.h>

#include "dynalo/symbol_helper.hpp"
#include "interface.h"

class foo : public IModule {
 public:
  foo();
  ~foo() override;

  void hello() override;
  folly::SemiFuture<int> get_fut() override;
  //  std::unique_ptr<folly::DMPSCQueue<int, false>> get_queue() override;
  bool set_queue(folly::DMPSCQueue<int, false>& queue) override;

 private:
  folly::Promise<int> promise_;
  //  std::unique_ptr<folly::DMPSCQueue<int, false>> queue_;
  folly::DMPSCQueue<int, false> queue_;
};
