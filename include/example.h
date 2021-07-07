#pragma once

#include "proto/addressbook.pb.h"

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

  static void PromptForAddress(tutorial::Person* person);
};

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest.h"
TEST_CASE("we can have tests in headers if we want") {
  Dummy d;
  CHECK(d.doSomething() == true);
}
#endif
