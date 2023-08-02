/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_multi.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgraefen <mgraefen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 14:27:44 by mgraefen          #+#    #+#             */
/*   Updated: 2023/08/02 12:39:31 by mgraefen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tcp_test.hpp"

std::string create_response() {
    const char* httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Connection: keep-alive\r\n\r\n";

    std::ifstream htmlFile("index.html");
    std::stringstream html_content;
    html_content << htmlFile.rdbuf();
    std::string final_response(httpResponse);
    final_response.append(html_content.str());
    return final_response;
}

int main(void){
	struct pollfd fds[MAX_CLIENTS];
	int nfds = 1;

	int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(listening_socket == -1){
		std::cout << "socket creation failed" << std::endl;
		return 1;
	}

	int opt = 1;
	if(setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){ // socket, level of operation(socketlvl), option, size of option
		std::cerr << "Error setting SO_REUSEADDR" << std::endl;
	}
	
	sockaddr_in servaddr;  // struct so save socketadresses(server)
  memset(&servaddr, 0, sizeof(servaddr));  // set to 0
  servaddr.sin_family = AF_INET;           // sets to IPv4
  servaddr.sin_addr.s_addr = htonl(
      INADDR_ANY);  // server adress is set with htonl - host to network long
  servaddr.sin_port = htons(1234);  // sets port with htons - host to short

  if (bind(listening_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) !=
      0) {  // binds socket to server,(incoming socket, casts serveraddr, size
            // of struct)
    std::cerr << "Could not bind socket\n";
    return 1;
  }

  if (listen(listening_socket, 5) !=
      0) {  // sets socket to passive, listens. 5 max queue length
    std::cerr << "Error in listen\n";
    return 1;
  }
	
	fds[0].fd = listening_socket; // inits first socket in socketarray to the first listenting socket
	fds[0].events = POLLIN; // sets to "check for incoming connections"
	fds[0].revents = 0; // init to 0

	while(true){
		int ret = poll(fds, nfds, -1); // polls the whole socket array with no timeout
		if(ret == -1){
			std::cout << "poll error" << std::endl;
			break;
		}
		
		for(int i = 0; i < nfds; i++){
			std::cout << "fds[" << i << "].revents: " << fds[i].revents << std::endl;
			if(fds[i].revents & POLLHUP)
				std::cout << "HANGUP "  << i << " " << std::endl;
			if(fds[i].revents & POLLIN) { // if incoming connection
				if(fds[i].fd == listening_socket) { 
					struct sockaddr_in clientaddr;
					socklen_t clientaddr_len = sizeof(clientaddr);
					int new_client_sock = accept(listening_socket, (struct sockaddr*)&clientaddr, &clientaddr_len);
					fds[nfds].fd = new_client_sock;
					fds[nfds].events = POLLIN;
					fds[nfds].revents = 0;
					++nfds;
				}
				else{
					char buffer[1024] = {0};  // creates buffer to store client msg
      		size_t bytes_read = read(fds[i].fd, buffer, sizeof(buffer));  // reads client msg
					if(bytes_read > 0){
						std::cout << "connection established " << i << " " << std::endl;
						std::string response = create_response();
						write(fds[i].fd, response.c_str(),
              response.size());
					}
					else if (fds[i].revents & POLLERR || bytes_read == 0){
						std::cout << "client closed connection "  << i << " " << std::endl;
						close(fds[i].fd);
						for(int j = i; j < nfds - 1; ++j){
							fds[j] = fds[j + 1];
						}
						--nfds;
						--i;
					}
					else
						std::cout << "some kind of error" << std::endl;
				}
			}  
		}
	}
}