#ifndef SERVERCLUSTER_HPP_
#define SERVERCLUSTER_HPP_

#include <iterator>
#include <set>
#include <vector>

#include "Config.hpp"
#include "TcpServer.hpp"
#include "Types.hpp"

class ServerCluster {
 public:
  ServerCluster(ConfigVector configs);
  ServerCluster();
  ~ServerCluster();
  ServerCluster(const ServerCluster& obj);
  ServerCluster& operator=(const ServerCluster& obj);

  void boot();

 private:
  void bootServer_();
  void serverLoop_();
  std::vector<TcpServer> cluster_;
};

#endif  // SERVERCLUSTER_HPP_
