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
    line.addError(parameter + " unexpected port");
    return 0;
  }
  int value = std::atoi(line[1].c_str());
  return value;
}

std::string parseServerName(Line &line) {
  (void)line;
  return "";
}

std::string parseIndex(Line &line) {
  (void)line;
  return "";
}

std::string parseRoot(Line &line) {
  (void)line;
  return "";
}

Location parseLocation(Line &line) {
  (void)line;
  return Location();
}

std::pair<int, std::string> parseErrorPage(Line &line) {
  (void)line;
  return std::make_pair(0, "");
}

bool parseAutoindex(Line &line) {
  (void)line;
  return true;
}

std::string parsePath(Line &line) {
  (void)line;
  return "";
}

std::pair<std::string, std::string> parseFastcgiPass(Line &line) {
  (void)line;
  return std::make_pair("", "");
}
