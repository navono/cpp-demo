#include "sharedLib/lib.h"
#include <iostream>

namespace lib {
void Hello::print() { std::cout << "Hello Shared Library!" << std::endl; }
}  // namespace lib
