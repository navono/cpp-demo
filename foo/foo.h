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

 private:
  folly::Promise<int> promise_;
};
