#ifndef SERVERCLUSTER_HPP_
#define SERVERCLUSTER_HPP_

#include <vector>

#include "Config.hpp"
#include "Types.hpp"
#include "TcpServer.hpp"

class ServerCluster {
 public:
  ServerCluster(ConfigVector configs);
  ServerCluster();
  ~ServerCluster();
  ServerCluster(const ServerCluster & obj);
  ServerCluster& operator=(const ServerCluster& obj);

 private:
  std::vector<TcpServer> cluster_;
};

#endif  // SERVERCLUSTER_HPP_
