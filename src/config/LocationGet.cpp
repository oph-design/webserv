#include "Location.hpp"

const bool& Location::getAutoindex() const { return this->autoindex_; }

const int& Location::getClientMaxBodySize() const {
  return this->clientMaxBodySize_;
}

const std::string& Location::getIndex() const { return this->index_; }

const std::string& Location::getPath() const { return this->path_; }

const std::string& Location::getRoot() const { return this->root_; }

const StringSet& Location::getMethods() const { return this->limitExcept_; }

const std::map<std::string, std::string>& Location::getFastcgiPass() const {
  return this->fastcgiPass_;
}

const ErrorMap& Location::getErrorPage() const { return this->errorPage_; }
