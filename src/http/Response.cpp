#include "Response.hpp"

#include <sstream>

#include "CgiConnector.hpp"
#include "ToString.hpp"

contentMap Response::fileTypes_ = Response::createTypeMap();

/*            constructors                  */

Response::Response() : body_("Server is online") {
  this->header_.insert(contentField("Content-Type", "text/plain"));
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Length", "16"));
}

Response::Response(const Request &request) {
  t_methodTypes method = request.getRequestMethodType();
  switch (method) {
    case 0:
      handleGetRequest_(request);
      break;
    case 1:
      handlePostRequest_(request);
      break;
    case 3:
      handleDeleteRequest_(request);
      break;
    default:
      this->status_.setCode(405);
      this->body_ = this->status_.getErrorBody();
  }
}

Response::Response(const Response &rhs) { *this = rhs; }

Response &Response::operator=(const Response &rhs) {
  this->body_ = rhs.body_;
  this->status_ = rhs.status_;
  this->header_ = rhs.header_;
  return (*this);
}

Response::~Response() { this->header_.clear(); }

/*            operators                  */

const std::string &Response::operator[](const std::string &key) {
  return (this->header_[key]);
}

const std::string &Response::operator[](const char *key) {
  return ((*this)[std::string(key)]);
}

std::ostream &operator<<(std::ostream &stream, const Response &resp) {
  stream << resp.getHeader() << resp.getBody();
  return (stream);
}

/*            Public Functions           */

const std::string Response::getHeader() const {
  std::stringstream header;

  header << "HTTP/1.1 " << this->status_ << "\r\n";
  for (contentMap::const_iterator it = this->header_.begin();
       it != this->header_.end(); ++it)
    header << it->first << ": " << it->second << "\r\n";
  header << "\r\n";
  return (header.str());
}

const std::string &Response::getBody() const { return (body_); }

const std::list<std::string> Response::getBodyChunked() const {
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

void Response::handleGetRequest_(const Request &request) {
  CgiConnector cgi(request);
  if (cgi.isCgi) {
    serveCgi_(cgi);
    return;
  }
  this->body_ = readBody_(request.getPath());
  std::string type = findType_(request.getPath());
  if (this->status_.getCode() > 399) this->body_ = status_.getErrorBody();
  std::string length = toString<std::size_t>(this->body_.length());

  this->header_.insert(contentField("Content-Type", type));
  this->header_.insert(contentField("Connection", "keep-alive"));
  this->header_.insert(contentField("Content-Length", length));
}

void Response::handlePostRequest_(const Request &request) {
  (void)request;
  this->status_.setCode(405);
  this->body_ = this->status_.getErrorBody();
}

void Response::handleDeleteRequest_(const Request &request) {
  (void)request;
  this->status_.setCode(405);
  this->body_ = this->status_.getErrorBody();
}

void Response::serveCgi_(CgiConnector &cgi) {
  cgi.makeConnection(this->status_);
  if (status_.getCode() > 399) {
    this->body_ = this->status_.getErrorBody();
    this->header_.insert(contentField("Content-Type", "text/html"));
    this->header_.insert(contentField("Connection", "keep-alive"));
    this->header_.insert(
        contentField("Content-Length", toString(this->body_.length())));
    return;
  }
  this->header_ = cgi.getHeader();
  this->body_ = cgi.getBody();
}

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
    this->status_.setCode(404);
  }
  std::stringstream content;
  content << file.rdbuf();
  file.close();
  return (content.str());
}

std::string Response::findType_(std::string url) {
  std::string extention;
  contentMap::iterator search;
  std::string type;

  if (!url.compare("/")) url.append("index.html");
  extention = url.substr(url.rfind(".") + 1, url.length());
  search = Response::fileTypes_.find(extention);
  if (search != Response::fileTypes_.end()) {
    type = Response::fileTypes_[extention];
  } else {
    this->status_.setCode(415);
    type = "text/html";
  }
  type.append("; charset=UTF-8");
  return (type);
}

/*            global funnctions                  */

contentMap Response::createTypeMap() {
  std::ifstream data("./resources/filetypes.csv");
  std::string field;
  std::string key;
  std::string value;
  contentMap fileTypes;

  while (data.is_open() && std::getline(data, field)) {
    try {
      key = field.substr(0, field.find(","));
      value = field.substr(field.find(",") + 1);
    } catch (std::exception &) {
      continue;
    }
    fileTypes.insert(contentField(key, value));
  }
  return (fileTypes);
}
