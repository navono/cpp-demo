#include "config.h"

#include <string>
#include <iostream>

namespace demo {
/**
 * Define default options
 */
bool Config::optDaemon = false;
bool Config::optDebug = false;

/**
 * Initialize all arguments
 */
cxxopts::Options Config::initializeArgs(int argc, char *argv[]) {
  cxxopts::Options options("demo.exe", "demo application, quite with press Ctrl+c 2 times.");

  options.add_options()("h,help", "Print this help message and exit.")(
      "d,daemon", "Launch as a daemon (in the background).")("debug", "Enable debugging mode.");

  try {
    auto result = options.parse(argc, argv);
    // Show help message if needed
    if (result.count("help")) {
      std::cout << options.help({""}) << std::endl;
      exit(EXIT_SUCCESS);
    }

    // Set all options
    Config::optDebug = result["debug"].as<bool>();
    Config::optDaemon = result["daemon"].as<bool>();

    return options;
  } catch (const cxxopts::exceptions::specification &e) {
    exit(EXIT_FAILURE);
  }
}
}  // namespace nuikit
