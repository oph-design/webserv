#ifndef ISNUMBER_HPP_
#define ISNUMBER_HPP_

#include <cctype>
#include <iterator>
#include <string>

template <typename Type>
bool isNumber(Type str) {
  for (typename Type::iterator iter = str.begin(); iter != str.end(); ++iter) {
    if (!std::isdigit(*iter)) return false;
  }
  return true;
}
#endif  // ISNUMBER_HPP_
