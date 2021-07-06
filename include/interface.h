#pragma once

#include <folly/concurrency/DynamicBoundedQueue.h>
#include <folly/futures/Future.h>

#include <zmq.hpp>

#include "dynalo/symbol_helper.hpp"

class IModule {
 public:
  virtual ~IModule() = default;  // <= important!
  virtual void hello() = 0;
  virtual void stop() = 0;
};

DYNALO_EXPORT IModule* DYNALO_CALL CreateFoo(std::shared_ptr<zmq::context_t> ctx, const std::string& subAddr);
