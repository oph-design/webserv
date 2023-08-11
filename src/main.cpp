#include <iostream>

#include "Response.hpp"
#include "Status.hpp"
#include "TcpServer.hpp"

int main(void) {
  Response::fillFileTypes();
  Status::fillStatusMap();
  TcpServer Server("localhost", 1234);
  Server.boot();
}
