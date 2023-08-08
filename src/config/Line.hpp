#ifndef LINE_HPP_
#define LINE_HPP_

#include <iostream>
#include <string>

class Line {
 public:
  Line();
  ~Line();
  Line(const Line& obj);
  Line(int lineNumber, std::string content);
  Line& operator=(const Line& obj);

  friend std::ostream& operator<<(std::ostream& stream, const Line& line);

 private:
  int lineNumber_;
  std::string content_;
  bool error_;
};

#endif  // LINE_HPP_
