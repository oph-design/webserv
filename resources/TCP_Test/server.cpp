#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <iostream>

bool pollSocket(int sockfd, short event, int timeout) {
  struct pollfd pfd; //struct to contain info for fd read with poll
  pfd.fd = sockfd; //setting up struct
  pfd.events = event; //setting up struct
  int ret = poll(&pfd, 1, timeout); // address of pfd as config, 1 fd to read, timeout in milliseconds
  if (ret == -1) {
    std::cerr << "poll failed: " << std::endl;
		return false;
  } else if (ret == 0) {
    std::cerr << "poll timed out" << std::endl;
		return false;
  } else {
    return pfd.revents & event; // if the event we want to occure and the event that happend the same -> return true;
  }
}

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

    // sends a simple response to browser
    if (pollSocket(sockfd, POLLIN, 5000)) { //check socket events with poll
      char buffer[1024] = {0};  // creates buffer to store client msg
      read(client_sockfd, buffer, sizeof(buffer));  // reads client msg

      // Simple check for an HTTP GET request
      if (strncmp(buffer, "GET", 3) == 0) {  // if it is GET method
        const char* httpResponse =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\n";  // response
                                                               // HTTP/1.1
                                                               // header
        write(client_sockfd, httpResponse,
              strlen(httpResponse));  // write response header to the socket
        std::ifstream htmlFile(
            "index.html");  // stream index.html file into ifstream
        if (htmlFile) {
          std::string line;
          while (std::getline(htmlFile, line)) {  // read line by line
            write(client_sockfd, line.c_str(),
                  line.size());  // write it into socket with c_str, which
                                 // converts it into a c type string
          }
        }
      } else {
        std::cout << "Received: " << buffer << "\n";
      }
    }

    close(client_sockfd);  // closes sockfd
  }

  close(sockfd);

  return 0;
}
