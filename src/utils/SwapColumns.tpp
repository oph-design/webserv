#include "Utils.hpp"

template <typename Key, typename Value>
std::map<Value, Key> swapColumns(std::map<Key, Value> input) {
  std::map<Value, Key> res;
  for (typename std::map<Key, Value>::iterator it = input.begin();
       it != input.end(); ++it)
    res.insert(std::pair<Value, Key>(it->second, it->first));
  return (res);
}
