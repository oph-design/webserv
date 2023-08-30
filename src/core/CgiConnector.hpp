#ifndef CGICONNECTOR_HPP
#define CGICONNECTOR_HPP

#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include "Request.hpp"
#include "Status.hpp"

typedef std::pair<std::string, std::string> envVar;
typedef std::map<std::string, std::string> envMap;

class CgiConnector {
 public:
  CgiConnector();
  CgiConnector(const Request& request);
  CgiConnector(const CgiConnector& rhs);
  CgiConnector& operator=(const CgiConnector& rhs);
  ~CgiConnector();

  void makeConnection(Status& status);
  std::map<std::string, std::string> getHeader() const;
  std::string getBody() const;
  static bool isCgi(std::string uri);

 private:
  envMap buildEnv_(const Request& request);
  char** envToString_();
  static void deleteEnv_(char** env);
  static pid_t waitAny(int* exitcode);
  void executeScript_(std::string path, int pipes[2]);
  void readOutput_(int pipes[2]);
  void writeReqBody();

  std::string respHeader_;
  std::string respBody_;
  std::string reqBody_;
  envMap env_;
};

#endif  // !CGICONNECTOR_HPP
