#include <iostream>

#include "ConfigFile.hpp"
#include "Signals.hpp"
#include "Webserver.hpp"

int main(int argc, char *argv[]) {
  implementSignalHandling();
  ConfigFile configFile;
  if (!configFile.openFile(argc, argv))
    return EXIT_FAILURE;
  ConfigVector configs = configFile.createConfig();
  if (!configFile.isValid()) {
    std::cout << configFile << std::endl;
    return EXIT_FAILURE;
  } else {
    Webserver Server(configs);
  }
  return EXIT_SUCCESS;
}
