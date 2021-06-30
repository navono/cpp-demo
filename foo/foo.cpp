#include "foo.h"

#include <interface.h>

#include <iostream>

using namespace std;

foo::foo() = default;

foo::~foo() = default;

void foo::hello() { std::cout << "hello from foo.dll" << std::endl; }

folly::SemiFuture<int> foo::get_fut() {
  promise_.setValue(100);
  return promise_.getSemiFuture();
}

IModule* DYNALO_CALL CreateFoo() { return new foo(); }
