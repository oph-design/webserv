#include <cstdlib>

#include "Socket.hpp"
#include "colors.hpp"

int parseCl(std::string buffer) {
  buffer = buffer.substr(buffer.find("Content-Length") + 16, buffer.length());
  return (atoi(buffer.c_str()));
}

int subtrHeader(std::string buffer) {
  buffer = buffer.substr(0, buffer.find("\r\n\r\n") + 4);
  return (buffer.length());
}

bool getTransferEncoding(std::string buffer) {
  return (buffer.find("Transfer-Encoding: chunked") != buffer.npos);
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
    reqstatus.chunked = getTransferEncoding(buffer);
    std::cout << GREEN << std::string(buffer, 400) << COLOR_RESET << std::endl;
  } else {
    reqstatus.buffer.append(std::string(buffer, bytes));
  }
  if (reqstatus.readBytes == reqstatus.clen ||
      (reqstatus.readBytes > reqstatus.clen && reqstatus.chunked)) {
    socket.setReqStatus();
    return (true);
  }
  reqstatus.pendingReceive = true;
  return (false);
}
