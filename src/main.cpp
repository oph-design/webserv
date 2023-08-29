#include <iostream>

#include "ConfigFile.hpp"
#include "ServerCluster.hpp"

int main(int argc, char *argv[]) {
  ConfigFile configFile;
  if (!configFile.openFile(argc, argv)) return EXIT_FAILURE;
  ConfigVector configs = configFile.createConfig();
  if (configFile.isValid() == false) {
    std::cout << configFile << std::endl;
    return EXIT_FAILURE;
  } else {
    ServerCluster cluster(configs);
    cluster.boot();
  }
  return EXIT_SUCCESS;
}
