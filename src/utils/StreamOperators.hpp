#ifndef STREAMOPERATORS_HPP_
#define STREAMOPERATORS_HPP_

#include <iostream>
#include <iterator>

template <typename Content>
std::ostream &operator<<(std::ostream &stream,
                         const std::vector<Content> container) {
  for (typename std::vector<Content>::const_iterator iter = container.begin();
       iter != container.end(); ++iter) {
    stream << *iter << "\n";
  }
  stream << std::flush;
  return stream;
}

template <typename First, typename Second>
std::ostream &operator<<(std::ostream &stream,
                         const std::map<First, Second> container) {
  for (typename std::map<First, Second>::const_iterator iter =
           container.begin();
       iter != container.end(); ++iter) {
    stream << iter->first << " : " << iter->second << "\n";
  }
  stream << std::flush;
  return stream;
}

#endif  // STREAMOPERATORS_HPP_
