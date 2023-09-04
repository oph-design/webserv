#include <iostream>

#include "ConfigFile.hpp"
#include "Webserver.hpp"

void signalHandling();

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

void signalHandling() {
#ifdef __linux__
  signal(SIGPIPE, SIG_IGN);
#endif
}
