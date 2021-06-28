#pragma once

#include "dynalo/symbol_helper.hpp"

class IModule {
 public:
  virtual ~IModule() = default;  // <= important!
  virtual void hello() = 0;
};

DYNALO_EXPORT IModule* DYNALO_CALL CreateFoo();
