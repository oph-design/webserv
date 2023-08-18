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
  ConfigVector configs = configFile.createConfig();
  if (configFile.isValid() == false) {
    std::cout << configFile << std::endl;
    return 1;
  } else {
    configFile.~ConfigFile();
    // TcpServer Server("localhost", 1234);
    TcpServer Server(configs[0], *configs[0].getListen().begin());
    Server.boot();
  }
  return 0;
}
