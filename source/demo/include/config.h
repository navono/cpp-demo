#include <cxxopts.hpp>

#include <string>

namespace demo {

class Config {
 public:
  static bool optDebug;
  static bool optDaemon;

  static void printBanner();
  static cxxopts::Options initializeArgs(int argc, char *argv[]);
};

}  // namespace nuikit