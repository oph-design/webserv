#ifndef HEADER_HPP_
#define HEADER_HPP_

#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

class Header {
 public:
  Header();
  Header(char buffer[BUFFER_SIZE]);
  Header(std::string &bufferString);
  ~Header();
  Header(const Header &obj);
  Header &operator=(const Header &obj);

  void addField(std::pair<std::string, std::string> &fieldWithContent);

  const std::string &operator[](const std::string &key) const;
  const std::string &operator[](const char *key) const;
  friend std::ostream &operator<<(std::ostream &stream, const Header &header);

 private:
  std::map<std::string, std::string> headerContent_;
};

#endif  // HEADER_HPP_
