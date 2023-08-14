#include <fstream>
#include <iostream>

#include "ConfigFile.hpp"
#include "Response.hpp"
#include "Status.hpp"
#include "TcpServer.hpp"
#include "Config.hpp"

int main(int argc, char *argv[]) {
  ConfigFile configFile;
  if (!configFile.openFile(argc, argv)) return 1;
  configFile.cleanContent();
  configFile.vaildateConfigFile();
  std::cout << configFile;
  std::vector<Config> configs = configFile.createConfig();
  TcpServer Server("localhost", 1234);
  Server.boot();
  return 0;
}
