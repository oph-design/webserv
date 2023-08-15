#ifndef CONFIGPARSING_HPP_
#define CONFIGPARSING_HPP_

#include <cstdlib>
#include <string>
#include <utility>

#include "Line.hpp"
#include "Location.hpp"

int parseListen(Line &line);
int parseCientMaxBodySize(Line &line);
std::string parseServerName(Line &line);
std::string parseIndex(Line &line);
std::string parseRoot(Line &line);
Location parseLocation(Line &line);
std::pair<int, std::string> parseErrorPage(Line &line);
bool parseAutoindex(Line &line);
std::string parsePath(Line &line);
std::pair<std::string, std::string> parseFastcgiPass(Line &line);

#endif  // CONFIGPARSING_HPP_
