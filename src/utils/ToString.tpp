#include "Utils.hpp"

template <typename Type>
std::string toString(Type obj) {
  std::stringstream ss;
  ss << obj;
  return ss.str();
}
