// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <fmt/format.h>
#include <folly/FBString.h>
#include <folly/Format.h>
#include <folly/Uri.h>
#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <iostream>

#include "example.h"
#include "exampleConfig.h"

/*
 * Simple main program that demonstrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
  std::cout << "C++ Boiler Plate v" << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "."
            << PROJECT_VERSION_PATCH << "." << PROJECT_VERSION_TWEAK << std::endl;

  folly::fbstring fs("World");

  // Use the default logger (stdout, multi-threaded, colored)
  spdlog::info(fmt::format("Hello: {}!", fs.toStdString()));

  std::system("cat ../LICENSE");

  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  //  Dummy d = Dummy();
  return Dummy::doSomething() ? 0 : -1;
}
