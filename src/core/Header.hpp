#ifndef HEADER_HPP_
#define HEADER_HPP_

#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

class Header {
  typedef enum e_method {
    GET = 0,
    POST = 1,
    DELETE = 2,
    OPTIONS = 3,
    HEAD = 4,
    PUT = 5,
    TRACE = 6,
    CONNECT = 7,
    INVALID = 8
  } t_method;

 public:
  Header();
  Header(char buffer[BUFFER_SIZE]);
  Header(std::string bufferString);
  ~Header();
  Header(const Header &obj);
  Header &operator=(const Header &obj);

  void addField(std::pair<std::string, std::string> &fieldWithContent);

  const std::string &operator[](const std::string &key) const;
  const std::string &operator[](const char *key) const;
  friend std::ostream &operator<<(std::ostream &stream, const Header &header);

 private:
  void parseRequestLine_(std::string &requestLine);

  std::map<std::string, std::string> headerContent_;
  std::string requestLine_;
  t_method method_;
  std::string methodString_;
  std::string URI_;
  std::string httpVersion_;
};

#endif  // HEADER_HPP_
