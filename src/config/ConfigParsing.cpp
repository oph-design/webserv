#include "ConfigParsing.hpp"

#include <cstdlib>
#include <string>
#include <utility>

#include "Config.hpp"
#include "Line.hpp"
#include "Location.hpp"
#include "Utils.hpp"

int ConfigParsing::parseListen(Line &line) {
  std::string parameter = "listen";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected port");
    return 0;
  }
  return std::atoi(line[1].c_str());
}

int ConfigParsing::parseCientMaxBodySize(Line &line, t_duplicates &duplicates) {
  std::string parameter = "client_max_body_size";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (!isNumber(line[1])) {
    line.addError(parameter + " unexpected number");
    return 0;
  }
  duplicates.clientMaxBodySize = true;
  return std::atoi(line[1].c_str());
}

std::string ConfigParsing::parseServerName(Line &line) {
  std::string parameter = "server_name";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  return line[1];
}

std::string ConfigParsing::parseIndex(Line &line, t_duplicates &duplicates) {
  std::string parameter = "index";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  duplicates.index = true;
  return line[1];
}

std::string ConfigParsing::parseRoot(Line &line, t_duplicates &duplicates) {
  std::string parameter = "root";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  duplicates.root = true;
  return line[1];
}

std::pair<int, std::string> ConfigParsing::parseErrorPage(Line &line) {
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

bool ConfigParsing::parseAutoindex(Line &line) {
  std::string parameter = "autoindex";
  if (line.words() != 2) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  if (line[1] == "on") {
    return true;
  } else if (line[1] == "off") {
    return false;
  } else {
    line.addError(parameter + " unexpected option");
  }
  return false;
}

std::string ConfigParsing::parsePath(Line &line) {
  std::string parameter = "location path";
  if (line.words() != 3) {
    line.addError(parameter + " unexpected arguments");
    return 0;
  }
  return line[1];
}

std::pair<std::string, std::string> ConfigParsing::parseFastcgiPass(
    Line &line) {
  std::string parameter = "error_page";
  if (line.words() != 3) {
    line.addError(parameter + " unexpected arguments");
    return std::make_pair("", "");
  }
  std::cout << "fastcgi " << line[1] << " : " << line[2] << std::endl;
  return std::make_pair(line[1], line[2]);
}
