#include "Config.hpp"

Config::Config() {
  this->client_max_body_size = 10000;
  this->server_name = "localhost";
  this->index = "index.html";
  this->root = "html/";
}

Config::~Config() {}

Config::Config(const Config& obj) { *this = obj; }

Config& Config::operator=(const Config& obj) {
  this->listen = obj.listen;
  this->client_max_body_size = obj.client_max_body_size;
  this->server_name = obj.server_name;
  this->index = obj.index;
  this->root = obj.root;
  this->locations = obj.locations;
  this->error_page = obj.error_page;
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
  for (std::vector<Location>::iterator iter = this->locations.begin();
       iter != this->locations.end(); ++iter) {
    if (iter->getDuplicates().clientMaxBodySize == false)
      iter->client_max_body_size = this->client_max_body_size;
    if (iter->getDuplicates().index == false) iter->index = this->index;
    if (iter->getDuplicates().root == false) iter->root = this->root;
    std::map<int, std::string> newErrorPage = this->error_page;
    newErrorPage.insert(iter->error_page.begin(), iter->error_page.end());
    iter->error_page = newErrorPage;
  }
}

void Config::fillAllLocations(std::vector<Config> &configs)
{
  for (std::vector<Config>::iterator iter = configs.begin(); iter != configs.end(); ++iter) {
    iter->fillLocations_();
  }
}

void Config::fillHostPort_(std::vector<Config>& configs) {
  for (std::vector<Config>::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    for (std::set<int>::iterator listenIter = configIter->listen.begin();
         listenIter != configIter->listen.end(); ++listenIter) {
      configIter->hostPort_.insert(configIter->server_name + ":" +
                                   toString(*listenIter));
    }
    if (configIter->hostPort_.size() == 0)
      configIter->hostPort_.insert(configIter->server_name + ":" +
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
    configIter->listen.clear();
    for (std::set<std::string>::iterator listenIter =
             configIter->hostPort_.begin();
         listenIter != configIter->hostPort_.end(); ++listenIter) {
      configIter->listen.insert(std::atoi(
          listenIter->substr(listenIter->find_last_of(':') + 1).c_str()));
    }
    if (configIter->hostPort_.size() == 0)
      configIter->hostPort_.insert(configIter->server_name + ":" +
                                   toString(STANDARD_PORT));
  }
}

void Config::deleteEmptyServer_(std::vector<Config>& configs) {
  std::vector<Config> newVector;
  for (std::vector<Config>::iterator configIter = configs.begin();
       configIter != configs.end(); ++configIter) {
    if (configIter->listen.size() != 0) newVector.push_back(*configIter);
  }
  configs = newVector;
}

std::ostream& operator<<(std::ostream& stream, const Config& config) {
  stream << "listen: " << config.listen << "\n";
  stream << "client_max_body_size: " << config.client_max_body_size << "\n";
  stream << "server_name: " << config.server_name << "\n";
  stream << "index: " << config.index << "\n";
  stream << "root: " << config.root << "\n";
  stream << "locations: " << config.locations << "\n";
  stream << "error_page: " << config.error_page << "\n";
  stream << "\n\n";
  stream << std::flush;
  return stream;
}
