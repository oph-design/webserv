#include "Response.hpp"

#include <sstream>

#include "ToString.hpp"

typeMap Response::fileTypes_;

Response::Response() : body_("Server is online") {
  this->header_.push_back(contentField("HTTP/1.1", "200 OK"));
  this->header_.push_back(contentField("Content-Type", "text/plain"));
  this->header_.push_back(contentField("Connection", "keep-alive"));
  this->header_.push_back(contentField("Content-Length", "16"));
}

Response::Response(Request request) : status_("200 OK") {
  this->body_ = readBody_(request.getPath());
  std::string type = findType_(request.getPath());
  std::string length = toString<std::size_t>(this->body_.length());

  this->header_.push_back(contentField(request.getHTTPVersion(), status_));
  this->header_.push_back(contentField("Content-Type", type));
  this->header_.push_back(contentField("Connection", "keep-alive"));
  this->header_.push_back(contentField("Content-Length", length));
}

Response::Response(const Response &rhs) { *this = rhs; }

Response &Response::operator=(const Response &rhs) {
  this->body_ = rhs.body_;
  this->header_ = rhs.header_;
  return (*this);
}

Response::~Response() { this->body_.clear(); }

std::string Response::getHeader() const {
  contentVector::const_iterator it = this->header_.begin();
  std::string res(it->first);

  res.append(" " + it->second + "\r\n");
  std::advance(it, 1);
  for (; it != this->header_.end(); ++it) {
    res.append(it->first + " : ");
    res.append(it->second + "\r\n");
  }
  res.append("\r\n");
  return (res);
}

std::string Response::getBody() const { return (body_); }

std::list<std::string> Response::getBodyChunked() const {
  size_t i = 0;
  std::string tmp = this->body_;
  std::list<std::string> res;

  for (std::string::iterator it = tmp.begin(); it < tmp.end(); ++it) {
    if (tmp.at(i) == '\n') {
      res.push_back(buildChunk_(tmp.substr(0, i + 1)));
      tmp = tmp.substr(i + 1, tmp.length());
      i = 0;
    }
  }
  if (tmp.length() > 0) res.push_back(buildChunk_(tmp));
  return (res);
}

/*            private functions                  */

std::string Response::buildChunk_(std::string line) {
  std::stringstream bytes;
  std::string res;

  bytes << std::hex << line.length();
  res = bytes.str() + "\r\n" + line + "\r\n";
  return (res);
}

std::string Response::readBody_(std::string dir) {
  if (!dir.compare("/")) dir.append("index.html");
  std::ifstream file(("./html" + dir).c_str());

  if (file.is_open()) {
    std::cout << "File opened successfully." << std::endl;
  } else {
    std::cerr << "Failed to open file." << std::endl;
    this->status_ = "404 Not Found";
  }
  if (status_.compare("200 OK")) {
    file.close();
    file.open(
        ("./html/" + status_.substr(0, status_.find(" ")) + ".html").c_str());
  }
  std::stringstream content;
  content << file.rdbuf();
  file.close();
  return (content.str());
}

std::string Response::findType_(std::string url) {
  std::string extention;
  typeMap::iterator search;
  std::string type;

  if (!url.compare("/")) url.append("index.html");
  extention = url.substr(url.rfind(".") + 1, url.length());
  search = Response::fileTypes_.find(extention);
  if (search != Response::fileTypes_.end()) {
    type = Response::fileTypes_[extention];
  } else {
    this->status_ = "415 Unsupported Media Type";
    this->body_ = readBody_("./html/415.html");
    type = "text/html";
  }
  type.append("; charset=UTF-8");
  return (type);
}

void Response::fillFileTypes() {
  Response::fileTypes_.insert(contentField("txt", "text/plain"));
  Response::fileTypes_.insert(contentField("html", "text/html"));
  Response::fileTypes_.insert(contentField("htm", "text/html"));
  Response::fileTypes_.insert(contentField("css", "text/css"));
  Response::fileTypes_.insert(contentField("js", "text/javascript"));
  Response::fileTypes_.insert(contentField("json", "apllication/json"));
  Response::fileTypes_.insert(contentField("xml", "apllication/xml"));
  Response::fileTypes_.insert(contentField("pdf", "apllication/pdf"));
  Response::fileTypes_.insert(contentField("zip", "apllication/zip"));
  Response::fileTypes_.insert(contentField("jpg", "image/jpeg"));
  Response::fileTypes_.insert(contentField("jpeg", "image/jpeg"));
  Response::fileTypes_.insert(contentField("png", "image/png"));
  Response::fileTypes_.insert(contentField("ico", "image/x-ico"));
  Response::fileTypes_.insert(contentField("mp3", "audio/mpeg"));
  Response::fileTypes_.insert(contentField("mp4", "video/mp4"));
}
