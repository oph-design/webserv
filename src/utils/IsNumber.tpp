#include "Utils.hpp"

template <typename Type>
bool isNumber(Type cont) {
  for (typename Type::iterator iter = cont.begin(); iter != cont.end();
       ++iter) {
    if (!std::isdigit(*iter)) return false;
  }
  return true;
}
