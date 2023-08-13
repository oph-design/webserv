#ifndef STATUS_HPP
#define STATUS_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "ToString.hpp"

typedef std::pair<int, std::string> statusPair;
typedef std::map<int, std::string> statusMap;

class Status {
 public:
  Status();
  Status(const Status& rhs);
  Status& operator=(const Status& rhs);
  ~Status();

  static statusMap createStatusMap();
  const int& getCode() const;
  void setCode(const int& newCode);
  friend std::ostream& operator<<(std::ostream& stream, const Status& status);
  std::string getErrorBody() const;

 private:
  int code_;
  static statusMap stats_;
};

#endif  // !STATUS_HPP
