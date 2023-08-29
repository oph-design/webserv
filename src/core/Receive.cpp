#include <cstdlib>

#include "Socket.hpp"

int parseCl(std::string buffer) {
  buffer = buffer.substr(buffer.find("Content-Length") + 16, buffer.length());
  return (atoi(buffer.c_str()));
}

int subtrHeader(std::string buffer) {
  buffer = buffer.substr(0, buffer.find("\r\n\r\n") + 4);
  return (buffer.length());
}

bool receiveRequest(Socket &socket, size_t &bytes) {
  char buffer[131072] = {0};
  t_reqStatus &reqstatus = socket.reqStatus;

  bytes = recv(socket.getSocketFd(), buffer, sizeof(buffer), O_NONBLOCK);
  reqstatus.readBytes += bytes;
  if (bytes == 0) return (false);
  if (reqstatus.pendingReceive == false) {
    reqstatus.clen = parseCl(buffer);
    reqstatus.buffer = std::string(buffer, bytes);
    reqstatus.readBytes -= subtrHeader(buffer);
  } else {
    reqstatus.buffer.append(std::string(buffer, bytes));
  }
  if (reqstatus.readBytes == reqstatus.clen) {
    socket.setReqStatus();
    return (true);
  }
  reqstatus.pendingReceive = true;
  return (false);
}
