#pragma once

#include "dynalo/symbol_helper.hpp"
#include "interface.h"

class foo : public IModule {
 public:
  foo();
  ~foo() override;

  void hello() override;
};
