#include <iostream>

#include "ConfigFile.hpp"
#include "Webserver.hpp"

int main(int argc, char *argv[]) {
  ConfigFile configFile;
  if (!configFile.openFile(argc, argv)) return EXIT_FAILURE;
  ConfigVector configs = configFile.createConfig();
  if (configFile.isValid() == false) {
    std::cout << configFile << std::endl;
    return EXIT_FAILURE;
  } else {
    Webserver Server(1234);
  }
  return EXIT_SUCCESS;
}
