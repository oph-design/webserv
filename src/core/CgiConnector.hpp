#ifndef CGICONNECTOR_HPP
#define CGICONNECTOR_HPP

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include "Response.hpp"

typedef std::pair<std::string, std::string> envVar;
typedef std::map<std::string, std::string> envMap;

class CgiConnector {
 public:
  typedef struct s_fds {
    int pipes[2];
    int stdIn;
    int stdOut;
  } t_fds;

  CgiConnector();
  CgiConnector(const Request& request);
  CgiConnector(const CgiConnector& rhs);
  CgiConnector& operator=(const CgiConnector& rhs);
  ~CgiConnector();

  int makeConnection();
  std::string getHeader_() const;
  std::string getBody_() const;

  bool isCgi;

 private:
  envMap buildEnv(const Request& request);
  char** envToString();
  void handlePipes(t_fds& fdStruct);
  void handlePipes(t_fds& fdStruct, int);
  void readOutput();
  bool checkCgi();
  std::string respHeader_;
  std::string respBody_;
  std::string reqBody_;
  envMap env_;
};

#endif  // !CGICONNECTOR_HPP

// REQUEST_METHOD:
//
// CONTENT_TYPE:
//
// CONTENT_LENGTH:
//
// QUERY_STRING:
//
// REQUEST_URI:
//
// SCRIPT_NAME:
//
// SERVER_PROTOCOL:
