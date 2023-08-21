#include "ServerCluster.hpp"

ServerCluster::ServerCluster(ConfigVector configs) {
}

ServerCluster::ServerCluster() {}

ServerCluster::~ServerCluster() {}

ServerCluster::ServerCluster(const ServerCluster & obj) { *this = obj; }

ServerCluster& ServerCluster::operator=(const ServerCluster& obj) {
  (void)obj;
  return *this;
}
