#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in servaddr;  // struct so save socketadresses(server)
  memset(&servaddr, 0, sizeof(servaddr));  // set to 0
  servaddr.sin_family = AF_INET;           // sets to IPv4
  servaddr.sin_addr.s_addr = htonl(
      INADDR_ANY);  // server adress is set with htonl - host to network long
  servaddr.sin_port = htons(1234);  // sets port with htons - host to short

  if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) !=
      0) {  // binds socket to server,(incoming socket, casts serveraddr, size
            // of struct)
    std::cerr << "Could not bind socket\n";
    return 1;
  }

  if (listen(sockfd, 5) !=
      0) {  // sets socket to passive, listens. 5 max queue length
    std::cerr << "Error in listen\n";
    return 1;
  }

  while (true) {
    sockaddr_in clientaddr;  // storage for client adrr
    socklen_t clientaddr_len =
        sizeof(clientaddr);  // storage for length of storage
    int client_sockfd = accept(
        sockfd, (struct sockaddr*)&clientaddr,
        &clientaddr_len);  // waits for incoming signals on sockfd, writes
                           // clientinfos in the corresponding structs

    char buffer[1024] = {0};
    read(client_sockfd, buffer,
         sizeof(buffer));  // writes incoming data in buffer
    std::cout << "Received: " << buffer << "\n";

    const char* msg = "Hello, client!";
    write(client_sockfd, msg, strlen(msg));  // writes message back to client

    close(client_sockfd);  // closes sockfd
  }

  close(sockfd);

  return 0;
}
