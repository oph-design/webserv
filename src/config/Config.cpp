#include "Config.hpp"

Config::Config() {
  this->clientMaxBodySize_ = FALLBACK_CLIENTMAXBODYSIZE;
  this->serverName_ = FALLBACK_SERVERNAME;
  this->index_ = FALLBACK_INDEX;
  this->root_ = FALLBACK_ROOT;
  Location location;
  this->locations_.push_back(location);
  this->timeout_ = FALLBACK_TIMEOUT;
}

Config::~Config() {}

Config::Config(const Config &obj) { *this = obj; }

Config &Config::operator=(const Config &obj) {
  this->listen_ = obj.listen_;
  this->clientMaxBodySize_ = obj.clientMaxBodySize_;
  this->serverName_ = obj.serverName_;
  this->index_ = obj.index_;
  this->root_ = obj.root_;
  this->locations_ = obj.locations_;
  this->errorPage_ = obj.errorPage_;
  this->port_ = obj.port_;
  this->timeout_ = obj.timeout_;
  return *this;
}

ConfigVector &Config::handleDuplicates_(ConfigVector &configs) {
  Config::fillHostPort_(configs);
  Config::makeHostPortUnique_(configs);
  Config::getPortsFromHostPort_(configs);
  Config::deleteEmptyServer_(configs);

  Config::fillAllLocations_(configs);
  return configs;
}

void Config::fillLocations_() {
  for (LocationVector::iterator iter = this->locations_.begin();
       iter != this->locations_.end(); ++iter) {
    if (iter->duplicates_.clientMaxBodySize == false)
      iter->clientMaxBodySize_ = this->clientMaxBodySize_;
    if (iter->duplicates_.index == false) iter->index_ = this->index_;
    if (iter->duplicates_.root == false) iter->root_ = this->root_;
    if (iter->duplicates_.upload_pass == false) iter->uploadPass_ = this->root_;
    if (iter->duplicates_.cgi_pass == false) iter->cgiPass_ = this->root_;
    if (iter->limitExcept_.empty()) iter->limitExcept_.insert("GET");
    ErrorMap newErrorPage = this->errorPage_;
    newErrorPage.insert(iter->errorPage_.begin(), iter->errorPage_.end());
    iter->errorPage_ = newErrorPage;
  }
}

void Config::fillAllLocations_(ConfigVector &configs) {
  for (ConfigVector::iterator iter = configs.begin(); iter != configs.end();
       ++iter) {
    iter->fillLocations_();
  }
}

void Config::fillHostPort_(ConfigVector &configs) {
  for (ConfigVector::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    for (std::set<int>::iterator listenIter = configIter->listen_.begin();
         listenIter != configIter->listen_.end(); ++listenIter) {
      configIter->hostPort_.insert(configIter->serverName_ + ":" +
                                   toString(*listenIter));
    }
    if (configIter->hostPort_.size() == 0)
      configIter->hostPort_.insert(configIter->serverName_ + ":" +
                                   toString(FALLBACK_PORT));
  }
}

void Config::makeHostPortUnique_(ConfigVector &configs) {
  StringSet portsInUse;
  for (ConfigVector::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    StringSet newSet;
    for (StringSet::iterator listenIter = configIter->hostPort_.begin();
         listenIter != configIter->hostPort_.end(); ++listenIter) {
      if (portsInUse.find(*listenIter) == portsInUse.end())
        newSet.insert(*listenIter);
    }
    portsInUse.insert(configIter->hostPort_.begin(),
                      configIter->hostPort_.end());
    configIter->hostPort_ = newSet;
  }
}

void Config::getPortsFromHostPort_(ConfigVector &configs) {
  for (ConfigVector::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    configIter->listen_.clear();
    for (StringSet::iterator listenIter = configIter->hostPort_.begin();
         listenIter != configIter->hostPort_.end(); ++listenIter) {
      configIter->listen_.insert(std::atoi(
          listenIter->substr(listenIter->find_last_of(':') + 1).c_str()));
    }
    if (configIter->hostPort_.size() == 0)
      configIter->hostPort_.insert(configIter->serverName_ + ":" +
                                   toString(FALLBACK_PORT));
  }
}

void Config::deleteEmptyServer_(ConfigVector &configs) {
  ConfigVector newVector;
  for (ConfigVector::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    if (configIter->listen_.size() != 0) newVector.push_back(*configIter);
  }
  configs = newVector;
}

std::ostream &operator<<(std::ostream &stream, const Config &config) {
  stream << "listen: " << config.listen_ << "\n";
  stream << "client_max_body_size: " << config.clientMaxBodySize_ << "\n";
  stream << "server_name: " << config.serverName_ << "\n";
  stream << "index: " << config.index_ << "\n";
  stream << "root: " << config.root_ << "\n";
  stream << "locations: " << config.locations_ << "\n";
  stream << "error_page: " << config.errorPage_ << "\n";
  stream << "\n";
  stream << std::flush;
  return stream;
}
