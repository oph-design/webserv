#include <iostream>

#include "ConfigFile.hpp"
#include "ServerCluster.hpp"

int main(int argc, char *argv[]) {
  ConfigFile configFile;
  if (!configFile.openFile(argc, argv)) return 1;
  ConfigVector configs = configFile.createConfig();
  if (configFile.isValid() == false) {
    std::cout << configFile << std::endl;
    return 1;
  } else {
    configFile.~ConfigFile();
    ServerCluster cluster(configs);
    cluster.boot();
  }
  return 0;
}
