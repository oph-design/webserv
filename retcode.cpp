/* void TcpServer::_existingConnection(int &i) {
  char buffer[1024] = {0};
  size_t bytes_read = 0;

  bytes_read = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
  if (bytes_read > 0) {
    Request request(buffer);
    Response resobj(request);
    pollSockets_[i].setTimestamp();
    std::cout << "connection established with socket " << _fds[i].fd << " "
              << std::endl;
    std::string response = resobj.getHeader() + resobj.getFullBody();
    std::cout << response << std::endl;              //// RESPONSE PRINT
    std::string header = resobj.getHeader();         //// HEADER
    std::list<std::string> body = resobj.getBody();  /// BODY LIST

                std::cout << "BYTES SEND: " << send(_fds[i].fd,
response.c_str(), response.size(), 0) << std::endl; std::cout << "CONTENT
LENGTH: " << response.size() << std::endl;
                //send(_fds[i].fd, header.c_str(), header.size(), 0);
    //send(_fds[i].fd, body.front().c_str(), body.front().size(), 0);

    if (body.size() == 1 && pollSockets_[i].pendingSend == false) {
      send(_fds[i].fd, header.c_str(), header.size(), 0);
      send(_fds[i].fd, body.front().c_str(), body.front().size(), 0);
    } else {
                                handleSegmentedTransmission(i, body, header);
    }
  } else if (bytes_read == 0 || !isKeepAlive(pollSockets_[i])) {
    std::cout << "client closed connection on socket " << _fds[i].fd << " "
              << std::endl;
    closeConnection_(pollSockets_[i], _fds[i], i);
  }
}

void TcpServer::handleSegmentedTransmission(int &i, std::list<std::string>
body, std::string header){ if (pollSockets_[i].pendingSend == false) {
                        pollSockets_[i].response_ = body;
                        pollSockets_[i].it = pollSockets_[i].response_.begin();
                        send(_fds[i].fd, header.c_str(), header.size(), 0);
                        send(_fds[i].fd, pollSockets_[i].it->c_str(),
                                                pollSockets_[i].it->size(), 0);
                        pollSockets_[i].it++;
                        pollSockets_[i].pendingSend = true;
                        return ;
                } else if (pollSockets_[i].it ==
pollSockets_[i].response_.end()) { pollSockets_[i].pendingSend = false; } else
if (pollSockets_[i].pendingSend == true) { send(_fds[i].fd,
pollSockets_[i].it->c_str(), pollSockets_[i].it->size(), 0);
                        pollSockets_[i].it++;
                }
}

void TcpServer::sendFile_(int fd, std::list<std::string> body) {
   size_t totalSend = 0;
    while (totalSend < response.size()) {
      size_t toSend =
          std::min(response.size() - totalSend, static_cast<size_t>(512));
                  size_t sent = send(fd, response.data() + totalSend, toSend,
    0); if(sent < 0){ std::cerr << "Error in response send" << std::endl; return
    ;
                  }
                  totalSend += sent;
    }
  for (std::list<std::string>::iterator it = body.begin(); it != body.end();
       it++) {
    // std::cout << *it << std::endl;
    send(fd, it->c_str(), it->size(), 0);
    usleep(2000);
  }
} */