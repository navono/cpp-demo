#pragma once

#include <folly/concurrency/DynamicBoundedQueue.h>
#include <folly/futures/Future.h>

#include "dynalo/symbol_helper.hpp"

class IModule {
 public:
  virtual ~IModule() = default;  // <= important!
  virtual void hello() = 0;

  virtual folly::SemiFuture<int> get_fut() = 0;
  //  virtual std::unique_ptr<folly::DMPSCQueue<int, false>> get_queue() = 0;
  virtual bool set_queue(folly::DMPSCQueue<int, false>& queue) = 0;
};

DYNALO_EXPORT IModule* DYNALO_CALL CreateFoo();
