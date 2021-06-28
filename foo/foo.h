#pragma once

#include "interface.h"
#include "dynalo/symbol_helper.hpp"

class foo : public IModule {
 public:
  foo();
  ~foo() override;

  void hello() override;
};
