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
  std::string getHeader_() const;
  std::string getBody_() const;

  bool isCgi;

 private:
  class InOutHandler {
   public:
    InOutHandler();
    ~InOutHandler();
    void dupInChild();
    void dupInParent();

   private:
    int pipes_[2];
    int stdIn_;
    int stdOut_;
  };

  envMap buildEnv_(const Request& request);
  char** envToString_();
  static void deleteEnv_(char** env);
  void executeScript_(std::string path, InOutHandler& io);
  void readOutput_();
  bool checkCgi_();

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
