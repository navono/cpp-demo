#ifndef DEMO_LIB_H
#define DEMO_LIB_H

#include "export.hpp"

namespace lib {
class SHAREDLIB_EXPORT Hello {
 public:
  void print();
};

}  // namespace lib

#endif  // DEMO_LIB_H
