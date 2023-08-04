#include <iostream>
#include "TcpServer.hpp"

int main(void) {
  TcpServer Server("localhost", 1234);
  Server.boot();
}
