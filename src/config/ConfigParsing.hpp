#ifndef CONFIGPARSING_HPP_
#define CONFIGPARSING_HPP_

#include <cstdlib>
#include <string>
#include <utility>

#include "Line.hpp"
#include "Location.hpp"

// in Location and Server
int parseCientMaxBodySize(Line &line, t_duplicates &duplicates);
std::string parseIndex(Line &line, t_duplicates &duplicates);
std::string parseRoot(Line &line, t_duplicates &duplicates);
std::pair<int, std::string> parseErrorPage(Line &line);

// in Server
int parseListen(Line &line);
std::string parseServerName(Line &line);
Location parseLocation(Line &line);

// in Location
std::string parsePath(Line &line);
bool parseAutoindex(Line &line);
std::pair<std::string, std::string> parseFastcgiPass(Line &line);

#endif  // CONFIGPARSING_HPP_
