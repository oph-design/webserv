#include "Location.hpp"

const std::string& Location::getRewrite() const { return this->rewrite_; }

const bool& Location::getAutoindex() const { return this->autoindex_; }

const int& Location::getClientMaxBodySize() const {
  return this->clientMaxBodySize_;
}

const std::string& Location::getIndex() const { return this->index_; }

const std::string& Location::getPath() const { return this->path_; }

const std::string& Location::getRoot() const { return this->root_; }

const std::string& Location::getUploadPass() const { return this->uploadPass_; }

const StringSet& Location::getMethods() const { return this->limitExcept_; }

const std::string& Location::getCgiPass() const { return this->cgiPass_; }

const ErrorMap& Location::getErrorPage() const { return this->errorPage_; }
