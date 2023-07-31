#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int main(int argc, char **argv) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  struct addrinfo hints, *res;            // structs to save adress info
  std::memset(&hints, 0, sizeof(hints));  // set struct to zero
  hints.ai_family = AF_INET;              // sets the ai_family to IPv4
  hints.ai_socktype = SOCK_STREAM;        // sets the socket type tp 'stream'

  int status =
      getaddrinfo(argv[1], argv[2], &hints,
                  &res);  // gets adress infos as defined in hints out of argv 1
                          // & 2 and saves the data in res pointer
  if (status != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(status)
              << "\n";  // checks if getadrr was succesful
    return 1;
  }
  if (connect(sock, res->ai_addr, res->ai_addrlen) !=
      0) {  // tries to use socket sock with the IP/Port & size of adr struct
            // provided in 'res.
    std::cerr << "Could not connect to server\n";
    freeaddrinfo(
        res);  // if fail, it frees res which was allocated by getaddrinfo
    return 1;
  }

  const char *msg = "Hello, Ole!";
  if (send(sock, msg, std::strlen(msg), 0) ==
      -1) {  // tries to send 'msg' over the socket 'sock'
    std::cerr << "Could not send message\n";
    freeaddrinfo(res);  // if not succesful, free res
    return 1;
  }

  char buffer[1024] = {0};
  if (recv(sock, buffer, sizeof(buffer), 0) == -1) {
    std::cerr << "Could not receive message\n";
    freeaddrinfo(res);
    return 1;
  }

  std::cout << "Received: " << buffer << "\n";

  freeaddrinfo(res);
  close(sock);

  return 0;
}