#ifndef DEMO_LIB_H
#define DEMO_LIB_H

#include "export.hpp"

#include <cppzmq/zmq.hpp>
#include <cppzmq/zmq_addon.hpp>

namespace lib {
class SHAREDLIB_EXPORT Hello {
 public:
  [[noreturn]] void sayHello(zmq::context_t *ctx, const std::string &addr);
};

}  // namespace lib

#endif  // DEMO_LIB_H
