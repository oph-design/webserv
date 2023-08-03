#include "Header.hpp"

Header::Header() {
  this->headerContent_.insert(
      std::make_pair(std::string("key1"), std::string("content1")));
  this->headerContent_.insert(
      std::make_pair(std::string("key2"), std::string("content2")));
  this->headerContent_.insert(
      std::make_pair(std::string("key3"), std::string("content3")));
  this->headerContent_.insert(
      std::make_pair(std::string("key4"), std::string("content4")));
}

Header::Header(char buffer[BUFFER_SIZE]) {
  std::string string(buffer);
  *this = Header(string);
}

// Header::Header(std::string &bufferString) {
//   std::string::iterator iter = bufferString.begin();
//   for (; iter != bufferString.end(); ++iter) {
//     this->requestLine_ += *iter;
//     if ((int)*iter == 13 && (int)*std::next(iter) == 10) {
//       iter += 2;
//       break;
//     }
//   }
//   std::string line;
//   for (; iter < bufferString.end(); ++iter) {
//     if ((int)*iter == 13)
//       continue;
//     line += *iter;
//     if ((int)*iter == 10) {
//       iter += 2;
//       std::pair<std::string, std::string> keyPair;
//       keyPair.first = line.substr(0, line.find(": "));
//       keyPair.second = line.substr(line.find(": "));
//       this->headerContent_.insert(keyPair);
//       line.clear();
//     }
//   }
// }

Header::Header(std::string bufferString) {
  // for (std::string::iterator iter = bufferString.begin(); iter != bufferString.end(); iter += bufferString.find("\n\r", iter - bufferString.begin()) + 2) {
  //   // std::pair<std::string, std::string> pair;
  //   // pair.first = bufferString.substr(bufferString.find(": ", iter - bufferString.begin()));
  //   // pair.second = bufferString.substr(bufferString.find(": ", iter - bufferString.begin(), bufferString.find("\10\13")));
  //   // this->headerContent_.insert(pair);
  //   std::cout << "line" << std::endl;
  // }

  std::stringstream ss(bufferString);
  std::string firstLine;
  std::getline(ss, firstLine);
  firstLine.erase('\r');
  std::cout << "--" << firstLine << "--" << std::endl;
  while (true) {
    // std::string line;
    char buffer[100] = {0};
    ss.getline(buffer, 100);
    std::string line(buffer);
    line.erase('\r');
    if (line.size() == 0)
      break ;
    std::cout << "--" << line << "--" << std::endl;
  }
  // strea
}

Header::~Header() {}

Header::Header(const Header &obj) { *this = obj; }

Header &Header::operator=(const Header &obj) {
  this->headerContent_ = obj.headerContent_;
  return *this;
}

const std::string &Header::operator[](const std::string &key) const {
  return this->headerContent_.at(key);
}

const std::string &Header::operator[](const char *key) const {
  std::string stringKey(key);
  return this->headerContent_.at(stringKey);
}

std::ostream &operator<<(std::ostream &stream, const Header &header) {
  for (std::map<std::string, std::string>::const_iterator iter =
           header.headerContent_.begin();
       iter != header.headerContent_.end(); ++iter) {
    stream << "[" << iter->first << "=" << iter->second << "]";
    if (std::next(iter) != header.headerContent_.end()) stream << ", ";
  }
  return stream;
}
