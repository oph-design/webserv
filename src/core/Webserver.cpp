#include "Webserver.hpp"

Webserver::Webserver(const Webserver &rhs) { *this = rhs; }

Webserver::~Webserver() {}

Webserver::Webserver(int port)
        : socketNum_(0), serverSocketNum_(0), clientSocketNum_(0), socketOpt_(1) {
    for (size_t i = 0; i < MAX_CLIENTS; i++) {
        fds_[i].fd = -1;
        fds_[i].events = POLLIN;
        fds_[i].revents = 0;
    }
    createServerSocket_(Sockets_[socketNum_], port);
    startServerRoutine_();
}

Webserver &Webserver::operator=(const Webserver &rhs) {
    (void) rhs;
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

    if (setsockopt(serverSocket.listeningSocket_,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &serverSocket.socketOpt_,
                   sizeof(serverSocket.socketOpt_)) == -1) {
        error_("Error: Setting SO_REUSEADDR");
    }

    if (setsockopt(serverSocket.listeningSocket_, SOL_SOCKET, SO_REUSEPORT,
                   &serverSocket.socketOpt_, sizeof(serverSocket.socketOpt_)) == -1) {
        error_("Error: Setting SO_REUSEPORT");
    }

    if (bind(serverSocket.listeningSocket_, (struct sockaddr *) &serverSocket.servaddr_,
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

int Webserver::getServerListen() {
    for (size_t i = 0; i < socketNum_; i++) {
        if (this->Sockets_[i].socketType_ == SERVER)
            return this->Sockets_[i].listeningSocket_;
    }
    return -1;
}

void Webserver::createClientSocket_(Socket &clientSocket) {
    socklen_t boundServerAdress_len = sizeof(clientSocket.boundServerAdress_);
    int new_client_sock;
    if ((new_client_sock = accept(clientSocket.fd_,
                                  (struct sockaddr *) &clientSocket.boundServerAdress_, &boundServerAdress_len)) == -1)
        error_("Accept Error");
    std::cout << new_client_sock << std::endl;
    if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
        error_("Error: Setting socket to nonblocking");
    }
    this->Sockets_[socketNum_].fd_ = new_client_sock;
    this->fds_[socketNum_].fd = new_client_sock;
    this->Sockets_[socketNum_].inUse_ = true;
    this->Sockets_[socketNum_].socketIndex_ = socketNum_;
    this->Sockets_[socketNum_].socketType_ = CLIENT;
    this->Sockets_[socketNum_].setTimestamp();
    socketNum_++;
    clientSocketNum_++;

    std::cout << "Connection Established with " << this->Sockets_[socketNum_ - 1].fd_ << std::endl;
}

void Webserver::startServerRoutine_() {
    while (true) {
        int ret = poll(this->fds_, this->socketNum_, 10000);
        if (ret == -1)
            error_("poll error");
        checkPending_();
        checkTimeoutClients();
        for (size_t i = 0; i < socketNum_; i++) {
            if (this->fds_[i].revents == POLLIN) {
                if (Sockets_[i].socketType_ == SERVER)
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

void Webserver::sendResponse_(Socket &socket, pollfd &pollfd, size_t &i) {
    socket.dataSend_ = send(socket.fd_, socket.response_.c_str(),
                            socket.response_.size(), 0);
    if (socket.dataSend_ < socket.response_.size()) {
        socket.pendingSend_ = true;
        socket.response_ = socket.response_.substr(socket.dataSend_);
        socket.setTimestamp();
        pollfd.events = POLLOUT;
    } else {
        socket.pendingSend_ = false;
        socket.setTimestamp();
        pollfd.events = POLLIN;
        if (socket.getKeepAlive() == false)
            closeConnection_(socket, pollfd, i);
    }
}

bool Webserver::existingConnection_(Socket &socket, pollfd &pollfd, size_t &i) {
    size_t currentBytes;
    if (receiveRequest(socket, currentBytes) || socket.pendingSend_) {
        std::cout << "connection on socket " << socket.fd_ << std::endl;
        socket.setTimestamp();
        if (socket.pendingSend_ == false)
            socket.response_ = this->createResponse_(socket.reqStatus.buffer);
        this->sendResponse_(socket, pollfd, i);
    }
    if (currentBytes == static_cast<std::size_t>(-1) &&
        (errno == EWOULDBLOCK || errno == EAGAIN)) {
        std::cout << "BLOCKER: " << socket.fd_ << std::endl;
    } else if (currentBytes == 0 && socket.reqStatus.pendingReceive == false) {
        if (socket.getKeepAlive() == false)
            closeConnection_(socket, pollfd, i);
        return false;
    }
    return true;
}

void Webserver::closeConnection_(Socket &socket, pollfd &pollfd, size_t &i) {
    std::cout << "Connection closing on Socket " << socket.fd_ << std::endl;
    close(pollfd.fd);
    socket.inUse_ = false;
    for (size_t j = i; j < MAX_CLIENTS - 1; ++j) {
        this->fds_[j] = this->fds_[j + 1];
        this->Sockets_[j] = this->Sockets_[j + 1];
    }
    this->fds_[MAX_CLIENTS - 1].fd = -1;
    this->Sockets_[MAX_CLIENTS - 1].setIdle();
    socketNum_--;
    clientSocketNum_--;
}

void Webserver::checkPending_() {
    for (size_t i = 0; i < socketNum_; ++i) {
        if (this->Sockets_[i].pendingSend_ == true) {
            this->sendResponse_(this->Sockets_[i], fds_[i], i);
            return;
        }
    }
}

void Webserver::checkTimeoutClients() {
    for (size_t i = 0; i < socketNum_; ++i) {
        if (this->Sockets_[i].socketType_ == CLIENT && this->Sockets_[i].checkTimeout() == true) {
            std::cout << "Timeout of Client Socket: " << this->Sockets_[i].fd_ << std::endl;
            closeConnection_(this->Sockets_[i], this->fds_[i], i);
        }
    }
}


void Webserver::error_(std::string error) {
    std::cerr << error << std::endl;
    exit(EXIT_FAILURE);
}
