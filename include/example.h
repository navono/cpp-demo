#pragma once

#include <quill/Quill.h>

#include <zmq.hpp>

/**
 * This is a dummy class to demonstrate features of the boiler plate.
 */
class Dummy {
 public:
  /**
   * Default constructor for Dummy (does nothing).
   */
  Dummy();
  /**
   * Returns a bool.
   * @return Always True.
   */
  static bool doSomething();
};

void publisherThread(const std::shared_ptr<quill::Logger>& logger, zmq::context_t* ctx);

void xPubSubProxy(const std::string& xPub_addr, const std::string& xSub_addr,
                  const std::shared_ptr<quill::Logger>& logger, const std::shared_ptr<zmq::context_t>& subCtx);

void xSubscriber(const std::string& xPub_addr, const std::shared_ptr<quill::Logger>& logger,
                 const std::shared_ptr<zmq::context_t>& subCtx);

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest.h"
TEST_CASE("we can have tests in headers if we want") {
  Dummy d;
  CHECK(d.doSomething() == true);
}
#endif
