#include <cctype>
#include <iterator>
#include <string>

bool isNumber(std::string str) {
  for (std::string::iterator iter = str.begin(); iter != str.end(); ++iter) {
    if (!std::isdigit(*iter)) return false;
  }
  return true;
}
