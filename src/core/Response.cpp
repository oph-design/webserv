#include "Response.hpp"

typeMap Response::fileTypes_;

Response::Response() : body_("Server is online") {
  header_.push_back(contentField("HTTP/1.1", "200 OK"));
  header_.push_back(contentField("Content-Type", findType_(".txt")));
  header_.push_back(contentField("Connection", "keep-alive"));
  header_.push_back(contentField("Content-Length", "16"));
}

Response::Response(Request request) : body_(readBody_(request.getURI())) {
  std::string length = std::to_string(body_.length());

  header_.push_back(contentField(request.getHTTPVersion(), "200 OK"));
  header_.push_back(contentField("Content-Type", findType_(request.getURI())));
  header_.push_back(contentField("Connection", "keep-alive"));
  header_.push_back(contentField("Content-Length", length));
}

Response::Response(const Response &rhs) { *this = rhs; }

Response &Response::operator=(const Response &rhs) {
  this->body_ = rhs.body_;
  this->header_ = rhs.header_;
  return (*this);
}

Response::~Response() { this->body_.clear(); }

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
  return (res);
}

/*            private functions                  */

std::string Response::readBody_(std::string dir) {
  std::ifstream htmlFile("." + dir);

  if (htmlFile.is_open()) {
    std::cout << "File opened successfully." << std::endl;
  } else {
    std::cerr << "Failed to open file." << std::endl;
  }
  std::stringstream html_content;
  html_content << htmlFile.rdbuf();
  return (html_content.str());
}

std::string Response::findType_(std::string url) {
  std::string extention;
  typeMap::iterator search;
  std::string type;

  extention = url.substr(url.rfind(".") + 1, url.length());
  search = fileTypes_.find(extention);
  if (search != fileTypes_.end()) {
    type = fileTypes_[extention];
  } else {
    type = "text/plain";
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
  fileTypes_.insert(contentField("mp3", "audio/mpeg"));
  fileTypes_.insert(contentField("mp4", "video/mp4"));
}
