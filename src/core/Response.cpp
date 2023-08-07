#include "Response.hpp"

#include <sstream>

typeMap Response::fileTypes_;

Response::Response() {
  header_.push_back(contentField("HTTP/1.1", "200 OK"));
  header_.push_back(contentField("Content-Type", "text/plain"));
  header_.push_back(contentField("Connection", "keep-alive"));
  header_.push_back(contentField("Content-Length", "16"));
}

Response::Response(Request request) : status_("200 OK") {
  std::string newBody = readBody_(request.getURI());
  std::string type = findType_(request.getURI());
  std::string length = std::to_string(newBody.length());
  bodyFull_ = newBody;

  header_.push_back(contentField(request.getHTTPVersion(), status_));
  header_.push_back(contentField("Content-Type", type));
  header_.push_back(contentField("Connection", "keep-alive"));
  header_.push_back(contentField("Content-Length", length));
  chunkBody_(newBody);
}

Response::Response(const Response &rhs) { *this = rhs; }

Response &Response::operator=(const Response &rhs) {
  this->body_ = rhs.body_;
  this->header_ = rhs.header_;
  return (*this);
}

Response::~Response() { this->body_.clear(); }

std::list<std::string> Response::getBody() const { return (body_); }

std::string Response::getHeader() const {
  contentVector::const_iterator it = header_.begin();
  std::string res(it->first);

  res.append(" " + it->second + "\r\n");
  std::advance(it, 1);
  for (; it != header_.end(); ++it) {
    res.append(it->first + " : ");
    res.append(it->second + "\r\n");
  }
  res.append("\r\n");
  return (res);
}

std::string Response::getFullBody() const { return (bodyFull_); }

/*            private functions                  */

std::string Response::readBody_(std::string dir) {
  if (!dir.compare("/")) dir.append("index.html");
  std::ifstream file("./html" + dir);

  if (file.is_open()) {
    std::cout << "File opened successfully." << std::endl;
  } else {
    std::cerr << "Failed to open file." << std::endl;
    this->status_ = "404 Not Found";
  }
  if (status_.compare("200 OK")) {
    file.close();
    file.open("./html/" + status_.substr(0, status_.find(" ")) + ".html");
  }
  std::stringstream content;
  content << file.rdbuf();
  file.close();
  return (content.str());
}

void Response::chunkBody_(std::string newBody) {
  size_t i = 0;

  for (std::string::iterator it = newBody.begin(); it < newBody.end(); ++it) {
    if (i++ == CHUNKSIZE) {
      body_.push_back(newBody.substr(0, CHUNKSIZE));
      newBody = newBody.substr(CHUNKSIZE, newBody.length());
      i = 0;
    }
  }
  if (newBody.length() > 0) body_.push_back(newBody);
}

std::string Response::findType_(std::string url) {
  std::string extention;
  typeMap::iterator search;
  std::string type;

  if (!url.compare("/")) url.append("index.html");
  extention = url.substr(url.rfind(".") + 1, url.length());
  search = fileTypes_.find(extention);
  if (search != fileTypes_.end()) {
    type = fileTypes_[extention];
  } else {
    this->status_ = "415 Unsupported Media Type";
  }
  type.append("; charset=UTF-8");
  return (type);
}

void Response::fillFileTypes() {
  fileTypes_.insert(contentField("txt", "text/plain"));
  fileTypes_.insert(contentField("html", "text/html"));
  fileTypes_.insert(contentField("htm", "text/html"));
  fileTypes_.insert(contentField("css", "text/css"));
  fileTypes_.insert(contentField("js", "text/javascript"));
  fileTypes_.insert(contentField("json", "apllication/json"));
  fileTypes_.insert(contentField("xml", "apllication/xml"));
  fileTypes_.insert(contentField("pdf", "apllication/pdf"));
  fileTypes_.insert(contentField("zip", "apllication/zip"));
  fileTypes_.insert(contentField("jpg", "image/jpeg"));
  fileTypes_.insert(contentField("jpeg", "image/jpeg"));
  fileTypes_.insert(contentField("png", "image/png"));
  fileTypes_.insert(contentField("ico", "image/x-ico"));
  fileTypes_.insert(contentField("mp3", "audio/mpeg"));
  fileTypes_.insert(contentField("mp4", "video/mp4"));
}
