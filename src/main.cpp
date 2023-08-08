#include <iostream>

#include "Config.hpp"
#include "TcpServer.hpp"

int main(int argc, char *argv[]) {
  int confFD = Config::openConfigFile(argc, argv);
  (void)confFD;
  TcpServer Server("localhost", 1234);
  Server.boot();
}
