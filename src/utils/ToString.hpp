#ifndef TOSTRING_HPP_
#define TOSTRING_HPP_

#include <string>
#include <sstream>

template <typename Type>
std::string toString(Type obj) {
  std::stringstream ss;
  ss << obj;
  return ss.str();
}

#endif  // TOSTRING_HPP_
