#include "Config.hpp"

const int& Config::getClientMaxBodySize() const {
  return this->clientMaxBodySize_;
}

const std::string& Config::getServerName() const { return this->serverName_; }

const std::string& Config::getIndex() const { return this->index_; }

const std::string& Config::getRoot() const { return this->root_; }

const LocationVector& Config::getLocations() const { return this->locations_; }

const ErrorMap& Config::getErrorPage() const { return this->errorPage_; }

const int& Config::getPort() const { return this->port_; }

const Location& Config::getLocationByPath(std::string index) {
  for (LocationVector::const_iterator it = locations_.begin();
       it < locations_.end(); ++it) {
    if (it->getIndex() == index) return (*it);
  }
  this->locations_[0].index_ = index;
  this->locations_[0].uploadPass_ = index;
  this->locations_[0].root_ = index;
  return (this->locations_[0]);
}
