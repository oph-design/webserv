#include "ServerCluster.hpp"

ServerCluster::ServerCluster(ConfigVector configs) {
  for (ConfigVector::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    for (std::set<int>::iterator listenIter = configIter->getListen().begin();
         listenIter != configIter->getListen().end();
         std::advance(listenIter, 1)) {
      TcpServer server(*configIter, *listenIter);
      this->cluster_.push_back(server);
    }
  }
}

ServerCluster::ServerCluster() {}

ServerCluster::~ServerCluster() {}

ServerCluster::ServerCluster(const ServerCluster& obj) { *this = obj; }

ServerCluster& ServerCluster::operator=(const ServerCluster& obj) {
  this->cluster_ = obj.cluster_;
  return *this;
}

void ServerCluster::boot() {
  this->bootServer_();
  this->serverLoop_();
}

void ServerCluster::bootServer_() {
  for (std::vector<TcpServer>::iterator iter = this->cluster_.begin();
       iter != this->cluster_.end(); ++iter) {
    iter->bootServer_();
  }
}

void ServerCluster::serverLoop_() {
  while (true) {
    for (std::vector<TcpServer>::iterator iter = this->cluster_.begin();
         iter != this->cluster_.end(); ++iter) {
      iter->serverLoop_();
    }
  }
}
