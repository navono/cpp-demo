#pragma once

#include <folly/futures/Future.h>

#include "dynalo/symbol_helper.hpp"

class IModule {
 public:
  virtual ~IModule() = default;  // <= important!
  virtual void hello() = 0;

  virtual folly::SemiFuture<int> get_fut() = 0;
};

DYNALO_EXPORT IModule* DYNALO_CALL CreateFoo();
