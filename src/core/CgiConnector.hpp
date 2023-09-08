#ifndef CGICONNECTOR_HPP
#define CGICONNECTOR_HPP

#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <csignal>
#include <cstdio>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include "Location.hpp"
#include "PrintVerbose.hpp"
#include "Request.hpp"
#include "Status.hpp"
#include "colors.hpp"

#define TIMEOUT 1

typedef std::pair<std::string, std::string> envVar;
typedef std::map<std::string, std::string> envMap;

class CgiConnector {
 public:
  CgiConnector();
  CgiConnector(const Request &request, const std::string &path);
  CgiConnector(const CgiConnector &rhs);
  CgiConnector &operator=(const CgiConnector &rhs);
  ~CgiConnector();

  void makeConnection(Status &status);
  std::map<std::string, std::string> getHeader() const;
  std::string getBody() const;
  static bool isCgi(const std::string &path);

 private:
  static envMap buildEnv_(const Request &request);
  char **envToString_();
  static bool waitTimeouted(pid_t pid, int *exitcode);
  void executeScript_(const std::string &path, int pipes[2]);
  void readOutput_(int pipes[2]);
  void writeReqBody();

  std::string respHeader_;
  std::string respBody_;
  std::string reqBody_;
  std::string path_;
  envMap env_;
};

#endif  // !CGICONNECTOR_HPP
