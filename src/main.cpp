#include <fstream>
#include <iostream>

#include "ConfigFile.hpp"
#include "TcpServer.hpp"

int main(int argc, char *argv[]) {
  ConfigFile configFile;
  if (!configFile.openFile(argc, argv)) return 1;
  configFile.cleanContent();
  configFile.vaildateConfigFile();
  std::cout << configFile;
  TcpServer Server("localhost", 1234);
  Server.boot();
  return 0;
}
