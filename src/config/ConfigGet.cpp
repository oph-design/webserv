#include "Config.hpp"

const int &Config::getClientMaxBodySize() const {
  return this->clientMaxBodySize_;
}

const std::string &Config::getServerName() const { return this->serverName_; }

const std::string &Config::getIndex() const { return this->index_; }

const std::string &Config::getRoot() const { return this->root_; }

const LocationVector &Config::getLocations() const { return this->locations_; }

const ErrorMap &Config::getErrorPage() const { return this->errorPage_; }

const int &Config::getPort() const { return this->port_; }

const int &Config::getTimeout() const { return this->timeout_; }
