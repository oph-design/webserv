#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include <map>
#include <string>

class Location {
 public:
  Location();
  ~Location();
  Location(const Location& obj);
  Location& operator=(const Location& obj);

  std::string path;
  std::map<std::string, std::string> params;

 private:
};

#endif  // LOCATION_HPP_
