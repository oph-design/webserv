#include <fstream>
#include <iostream>

#include "Config.hpp"
#include "ConfigFile.hpp"
#include "Response.hpp"
#include "Status.hpp"
#include "TcpServer.hpp"

int main(int argc, char *argv[]) {
  ConfigFile configFile;
  if (!configFile.openFile(argc, argv)) return 1;
  std::vector<Config> configs = configFile.createConfig();
  if (configFile.isValid() == false) {
    std::cout << configFile << std::endl;
    return 1;
  } else {
    TcpServer Server("localhost", 1234);
    Server.boot();
  }
  return 0;
}
