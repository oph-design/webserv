#ifndef LOCATION_HPP_
#define LOCATION_HPP_

#include <map>
#include <string>
#include <vector>

class Location {
 public:
  Location();
  ~Location();
  Location(const Location& obj);
  Location& operator=(const Location& obj);

  bool autoindex;
  int client_max_body_size;
  std::string index;
  std::string path;
  std::string root;
  std::vector<std::string> limit_except;
  std::map<std::string, std::string> fastcgi_pass;
  std::map<int, std::string> error_page;

 private:
};

#endif  // LOCATION_HPP_
