#include <utility>
#include <string>

#include "Line.hpp"
#include "Location.hpp"

int parseListen(Line &line, bool &alreadyParsedListen) {
  (void)line;
  (void)alreadyParsedListen;
  return 0;
}

int parseCientMaxBodySize(Line &line) {
  (void)line;
  return 0;
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
