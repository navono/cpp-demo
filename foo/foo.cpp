#include "foo.h"

#include <iostream>

using namespace std;

foo::foo() = default;

foo::~foo() = default;

void foo::hello() { std::cout << "hello from foo.dll" << std::endl; }

DYNALO_EXPORT IModule* DYNALO_CALL CreateFoo() { return new foo(); }
