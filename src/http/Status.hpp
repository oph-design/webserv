#ifndef STATUS_HPP
#define STATUS_HPP

#include <cstdlib>
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
  Status& operator=(int newCode);
  ~Status();

  static statusMap createStatusMap();
  bool operator==(int rhs) const;
  bool operator<=(int rhs) const;
  bool operator>=(int rhs) const;
  bool operator>(int rhs) const;
  bool operator<(int rhs) const;
  std::string& operator>>(std::string& str);
  friend std::ostream& operator<<(std::ostream& stream, const Status& status);
  std::string getErrorBody() const;

 private:
  int code_;
  static statusMap stats_;
};

#endif  // !STATUS_HPP
