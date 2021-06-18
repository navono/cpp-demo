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
#include "threadPool.h"

#define USE_FOLLY 0

static void print_uri(const folly::fbstring& address) {
  const folly::Uri uri(address);
  const auto authority = folly::format("The authority from {} is {}", uri.fbstr(), uri.authority());
  std::cout << authority << std::endl;
}

/*
 * Simple main program that demonstrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
  std::cout << "C++ Boiler Plate v" << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "."
            << PROJECT_VERSION_PATCH << "." << PROJECT_VERSION_TWEAK << std::endl;

  //  std::string name = "World";
  folly::fbstring fs("World");
  std::cout << fmt::format("Hello: {}!", fs.toStdString()) << std::endl;

  std::system("cat ../LICENSE");

  // Use the default logger (stdout, multi-threaded, colored)
  spdlog::info("Hello, {}!", "World");

  // create color multi threaded logger
  auto console = spdlog::stdout_color_mt("console");
  auto err_logger = spdlog::stderr_color_mt("stderr");
  spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

  folly::ThreadedExecutor executor;
  folly::Promise<folly::fbstring> promise;

  folly::Future<folly::fbstring> future = promise.getSemiFuture().via(&executor);
  folly::Future<folly::Unit> unit = std::move(future).thenValue(print_uri);
  promise.setValue("https://conan.io/");
  std::move(unit).get();

  thread_pool_verify(24, 10000, 100000);

  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  //  Dummy d = Dummy();
  return Dummy::doSomething() ? 0 : -1;
}
