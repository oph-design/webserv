#include "Config.hpp"

Config::Config() {
  this->clientMaxBodySize_ = 10000;
  this->serverName_ = "localhost";
  this->index_ = "index.html";
  this->root_ = "html/";
}

Config::~Config() {}

Config::Config(const Config& obj) { *this = obj; }

Config& Config::operator=(const Config& obj) {
  this->listen_ = obj.listen_;
  this->clientMaxBodySize_ = obj.clientMaxBodySize_;
  this->serverName_ = obj.serverName_;
  this->index_ = obj.index_;
  this->root_ = obj.root_;
  this->locations_ = obj.locations_;
  this->errorPage_ = obj.errorPage_;
  return *this;
}

std::vector<Config>& Config::handleDuplicates(std::vector<Config>& configs) {
  fillHostPort_(configs);
  makeHostPortUnique_(configs);
  getPortsFromHostPort_(configs);
  deleteEmptyServer_(configs);

  fillAllLocations(configs);
  return configs;
}

void Config::fillLocations_() {
  for (std::vector<Location>::iterator iter = this->locations_.begin();
       iter != this->locations_.end(); ++iter) {
    if (iter->getDuplicates().clientMaxBodySize == false)
      iter->clientMaxBodySize_ = this->clientMaxBodySize_;
    if (iter->getDuplicates().index == false) iter->index_ = this->index_;
    if (iter->getDuplicates().root == false) iter->root_ = this->root_;
    std::map<int, std::string> newErrorPage = this->errorPage_;
    newErrorPage.insert(iter->errorPage.begin(), iter->errorPage.end());
    iter->errorPage = newErrorPage;
  }
}

void Config::fillAllLocations(std::vector<Config>& configs) {
  for (std::vector<Config>::iterator iter = configs.begin();
       iter != configs.end(); ++iter) {
    iter->fillLocations_();
  }
}

void Config::fillHostPort_(std::vector<Config>& configs) {
  for (std::vector<Config>::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    for (std::set<int>::iterator listenIter = configIter->listen_.begin();
         listenIter != configIter->listen_.end(); ++listenIter) {
      configIter->hostPort_.insert(configIter->serverName_ + ":" +
                                   toString(*listenIter));
    }
    if (configIter->hostPort_.size() == 0)
      configIter->hostPort_.insert(configIter->serverName_ + ":" +
                                   toString(STANDARD_PORT));
  }
}

void Config::makeHostPortUnique_(std::vector<Config>& configs) {
  std::set<std::string> portsInUse;
  for (std::vector<Config>::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    std::set<std::string> newSet;
    for (std::set<std::string>::iterator listenIter =
             configIter->hostPort_.begin();
         listenIter != configIter->hostPort_.end(); ++listenIter) {
      if (portsInUse.find(*listenIter) == portsInUse.end())
        newSet.insert(*listenIter);
    }
    portsInUse.insert(configIter->hostPort_.begin(),
                      configIter->hostPort_.end());
    configIter->hostPort_ = newSet;
  }
}

void Config::getPortsFromHostPort_(std::vector<Config>& configs) {
  for (std::vector<Config>::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    configIter->listen_.clear();
    for (std::set<std::string>::iterator listenIter =
             configIter->hostPort_.begin();
         listenIter != configIter->hostPort_.end(); ++listenIter) {
      configIter->listen_.insert(std::atoi(
          listenIter->substr(listenIter->find_last_of(':') + 1).c_str()));
    }
    if (configIter->hostPort_.size() == 0)
      configIter->hostPort_.insert(configIter->serverName_ + ":" +
                                   toString(STANDARD_PORT));
  }
}

void Config::deleteEmptyServer_(std::vector<Config>& configs) {
  std::vector<Config> newVector;
  for (std::vector<Config>::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    if (configIter->listen_.size() != 0) newVector.push_back(*configIter);
  }
  configs = newVector;
}

std::ostream& operator<<(std::ostream& stream, const Config& config) {
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
