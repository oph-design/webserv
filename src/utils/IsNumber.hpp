#ifndef ISNUMBER_HPP_
#define ISNUMBER_HPP_

#include <cctype>
#include <iterator>

template <typename Type>
bool isNumber(Type cont) {
  for (typename Type::iterator iter = cont.begin(); iter != cont.end();
       ++iter) {
    if (!std::isdigit(*iter)) return false;
  }
  return true;
}
#endif  // ISNUMBER_HPP_
