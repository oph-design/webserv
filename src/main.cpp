#include <iostream>

#include "ConfigFile.hpp"
#include "Signals.hpp"
#include "Webserver.hpp"

int main(int argc, char *argv[]) {
  signalHandling();
  ConfigFile configFile;
  if (!configFile.openFile(argc, argv)) return EXIT_FAILURE;
  ConfigVector configs = configFile.createConfig();
  if (configFile.isValid() == false) {
    std::cout << configFile << std::endl;
    return EXIT_FAILURE;
  } else {
    Webserver Server(configs);
  }
  return EXIT_SUCCESS;
}
