#include <fstream>
#include <iostream>

#include "Config.hpp"
#include "TcpServer.hpp"

int main(int argc, char *argv[]) {
  Config config;
  if (!config.openFile(argc, argv)) return 1;
  std::cout << config;
  TcpServer Server("localhost", 1234);
  Server.boot();
  return 0;
}
