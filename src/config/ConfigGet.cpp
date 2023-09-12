#include <cstring>

#include "Config.hpp"
#include "Utils.hpp"

const int &Config::getClientMaxBodySize() const {
  return this->clientMaxBodySize_;
}

const std::string &Config::getServerName() const { return this->serverName_; }

const std::string &Config::getIndex() const { return this->index_; }

const std::string &Config::getRoot() const { return this->root_; }

const LocationVector &Config::getLocations() const { return this->locations_; }

const ErrorMap &Config::getErrorPage() const { return this->errorPage_; }

const int &Config::getPort() const { return this->port_; }

const Location &Config::getLocationByPath(const std::string &index) {
  std::string tmp;
  for (LocationVector::const_iterator it = locations_.begin();
       it < locations_.end(); ++it) {
    if (it->getPath().empty()) continue;
    if (index.size() < it->getPath().size()) continue;
    tmp = index.substr(0, it->getPath().size());
    if (it->getPath() == tmp + '/') return (*it);
    if (!strcmp(tmp.c_str(), it->getPath().c_str())) return (*it);
  }
  return (this->locations_[0]);
}

const int &Config::getTimeout() const { return this->timeout_; }
