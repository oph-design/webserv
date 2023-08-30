#include "Webserver.hpp"

Webserver::Webserver(const Webserver &rhs) { *this = rhs; }

Webserver::~Webserver() {}

Webserver::Webserver(int port)
    : socketNum_(0), serverSocketNum_(0), clientSocketNum_(0), socketOpt_(1) {
	for(size_t i = 0; i < MAX_CLIENTS; i++){
		fds_[i].fd = -1;
		fds_[i].events = POLLIN;
		fds_[i].revents = 0;
	}
  createServerSocket_(Sockets_[socketNum_], port);
  startServerRoutine_();
}

Webserver &Webserver::operator=(const Webserver &rhs){
    (void)rhs;
    return *this;
}

void Webserver::createServerSocket_(Socket &serverSocket, int port) {

    memset(&serverSocket.servaddr_, 0, sizeof(serverSocket.servaddr_));
  serverSocket.servaddr_.sin_family = AF_INET;
  serverSocket.servaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
  serverSocket.servaddr_.sin_port = htons(port);

  serverSocket.listeningSocket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket.listeningSocket_ == -1) {
    error_("Error: Socket creation failed");
  }

  if (setsockopt(serverSocket.listeningSocket_, SOL_SOCKET, SO_REUSEADDR,
                 &serverSocket.socketOpt_, sizeof(serverSocket.socketOpt_)) == -1) {
    error_("Error: Setting SO_REUSEADDR");
  }

  if (setsockopt(serverSocket.listeningSocket_, SOL_SOCKET, SO_REUSEPORT,
                 &serverSocket.socketOpt_, sizeof(serverSocket.socketOpt_)) == -1) {
    error_("Error: Setting SO_REUSEPORT");
  }

  if (bind(serverSocket.listeningSocket_, (struct sockaddr *)&serverSocket.servaddr_,
           sizeof(serverSocket.servaddr_)) != 0) {
    error_("Error: Could not bind socket");
  }

  if (listen(serverSocket.listeningSocket_, 25) != 0) {
    error_("Error: Listen");
  }

  if (fcntl(serverSocket.listeningSocket_, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    error_("Error: Setting socket to nonblocking");
  }

  this->fds_[socketNum_].fd = serverSocket.listeningSocket_;
  serverSocket.fd_ = serverSocket.listeningSocket_;
  serverSocket.socketIndex_ = socketNum_;
  serverSocket.socketType_ = SERVER;
  this->socketNum_++;
  this->serverSocketNum_++;
}

int Webserver::getServerListen(){
    for (size_t i = 0; i < socketNum_; i++){
        if (this->Sockets_[i].socketType_ == SERVER)
            return this->Sockets_[i].listeningSocket_;
    }
    return -1;
}

void Webserver::createClientSocket_(Socket &clientSocket) {
    socklen_t boundServerAdress_len = sizeof(clientSocket.boundServerAdress_);
    int new_client_sock;
    if((new_client_sock = accept(clientSocket.fd_,
                                 (struct sockaddr *)&clientSocket.boundServerAdress_, &boundServerAdress_len) == -1))
        error_("Accept Error");
    if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
        error_("Error: Setting socket to nonblocking");
    }
    this->Sockets_[socketNum_].fd_ = new_client_sock;
    this->Sockets_[socketNum_].inUse = true;
    this->Sockets_[socketNum_].socketIndex_ = socketNum_;
    this->Sockets_[socketNum_].socketType_ = CLIENT;
    socketNum_++;
    clientSocketNum_++;
    std::cout << "Connection Established with " << clientSocket.fd_ << std::endl;
}

void Webserver::startServerRoutine_(){
    while(true){
        int ret = poll(this->fds_, this->socketNum_, 10000);
        if(ret == -1)
            error_("poll error");
        for(size_t i = 0; i < socketNum_; i++ ){
            if(this->fds_[i].revents == POLLIN){
                if(Sockets_[i].socketType_ == SERVER)
                    createClientSocket_(Sockets_[i]);
                else {
                    existingConnection_(Sockets_[i], fds_[i], i);
                }
            }

        }
    }
}

std::string Webserver::createResponse_(std::string buffer) {
    Request request(buffer);
    Response resobj(request);
    std::string response = resobj.getHeader() + resobj.getBody();
    return response;
}

void Webserver::sendResponse_(Socket &socket, pollfd &fd, size_t &i) {
    socket.dataSend = send(socket.fd_, socket.response_.c_str(),
                           socket.response_.size(), 0);
    if (socket.dataSend < socket.response_.size()) {
        socket.pendingSend = true;
        socket.response_ = socket.response_.substr(socket.dataSend);
        fd.events = POLLOUT;
    } else {
        socket.pendingSend = false;
        fd.events = POLLIN;
        closeConnection_(socket, fd, i);
    }
}

bool Webserver::existingConnection_(Socket &socket, pollfd &fd, size_t &i) {
    size_t currentBytes;
    if (receiveRequest(socket, currentBytes) || socket.pendingSend == true) {
        std::cout << "connection on socket " << socket.fd_ << std::endl;
        //socket.setTimestamp();
        if (socket.pendingSend == false)
            socket.response_ = this->createResponse_(socket.reqStatus.buffer);
        this->sendResponse_(socket, fd, i);
    }
    if (currentBytes == static_cast<std::size_t>(-1) &&
        (errno == EWOULDBLOCK || errno == EAGAIN)) {
        std::cout << "BLOCKER: " << socket.fd_ << std::endl;
    } else if (currentBytes == 0 && socket.reqStatus.pendingReceive == false) {
        std::cout << "client closed socket " << socket.fd_ << std::endl;
        return false;
    }
    return true;
}

void Webserver::closeConnection_(Socket &socket, pollfd &fd, size_t &i) {
    close(fd.fd);
    close(socket.fd_);
    socket.inUse = false;
    for (size_t j = i; j < socketNum_ - 1; ++j) {
        this->fds_[j] = this->fds_[j + 1];
    }
    socketNum_--;
    clientSocketNum_--;
    i--;

}

void Webserver::error_(std::string error) {
    std::cerr << error << std::endl;
    exit(EXIT_FAILURE);
}
