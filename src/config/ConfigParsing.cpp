#include <cstdlib>
#include <string>
#include <utility>

#include "Line.hpp"
#include "Location.hpp"
#include "Utils.hpp"

int parseListen(Line &line) {
  std::string parameter = "listen";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected port");
    return 0;
  }
  int value = std::atoi(line[1].c_str());
  return value;
}

int parseCientMaxBodySize(Line &line) {
  std::string parameter = "client_max_body_size";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected number");
    return 0;
  }
  int value = std::atoi(line[1].c_str());
  return value;
}

std::string parseServerName(Line &line) {
  std::string parameter = "server_name";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  return line[1];
}

std::string parseIndex(Line &line) {
  std::string parameter = "index";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  return line[1];
}

std::string parseRoot(Line &line) {
  std::string parameter = "root";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  return line[1];
}

std::pair<int, std::string> parseErrorPage(Line &line) {
  std::string parameter = "error_page";
  if (line.words() != 3) {
    line.addError(parameter + " unexpected arguments");
    return std::make_pair(0, "");
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected number");
    return std::make_pair(0, "");
  }
  return std::make_pair(std::atoi(line[1].c_str()), line[2]);
}

bool parseAutoindex(Line &line) {
  std::string parameter = "autoindex";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (line[1] == "on"){ return true;}
  else if (line[1] == "off"){ return false;}
  else{
    line.addError(parameter + " unexpected option");
  }
  return false;
}

std::string parsePath(Line &line) {
  (void)line;
  return "";
}

std::pair<std::string, std::string> parseFastcgiPass(Line &line) {
  std::string parameter = "error_page";
  if (line.words() != 3) {
    line.addError(parameter + " unexpected arguments");
    return std::make_pair("", "");
  }
  std::cout << "fastcgi "<< line[1] << " : " << line[2] << std::endl;
  return std::make_pair(line[1], line[2]);
}
