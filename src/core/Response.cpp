#include "Response.hpp"
#include <iterator>

Response::Response() : body_("Server is online") {
  header_.push_back(contentField("HTTP/1.1", "200 OK"));
  header_.push_back(contentField("Content-Type", "text/plain; charset=UTF-8"));
  header_.push_back(contentField("Connection", "keep-alive"));
  header_.push_back(contentField("Content-Length", "16"));
}

Response::Response(const Response &rhs) { *this = rhs; }

Response &Response::operator=(const Response &rhs) {
  this->body_ = rhs.body_;
  this->header_ = rhs.header_;
  return (*this);
}

Response::~Response() { this->body_.clear(); }

std::string Response::readBody_(std::string dir) {
  std::ifstream htmlFile(dir);

  if (htmlFile.is_open()) {
    std::cout << "html File opened successfully." << std::endl;
  } else {
    std::cerr << "Failed to open the html file." << std::endl;
  }
  std::stringstream html_content;
  html_content << htmlFile.rdbuf();
  return (html_content.str());
}

std::string Response::toString() const {
  contentVector::const_iterator it = header_.begin();
  std::string res(it->first);

  res.append(" " + it->second + "\r\n");
  std::advance(it, 1);
  for (; it != header_.end(); ++it) {
    res.append(it->first + " : ");
    res.append(it->second + "\r\n");
  }
  res.append("\r\n" + body_);
  std::cout << res << "\n";
  return (res);
}
