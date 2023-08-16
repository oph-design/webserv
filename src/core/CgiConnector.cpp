#include "CgiConnector.hpp"

CgiConnector::CgiConnector() {}

CgiConnector::CgiConnector(const Request& request) {
  if (request.getRequestBodyExists()) this->reqBody_ = request.getRequestBody();
  this->env_ = buildEnv_(request);
  this->isCgi = checkCgi_();
}

CgiConnector::CgiConnector(const CgiConnector& rhs) { *this = rhs; }

CgiConnector& CgiConnector::operator=(const CgiConnector& rhs) {
  this->respBody_ = rhs.respBody_;
  this->respHeader_ = rhs.respHeader_;
  this->env_ = rhs.env_;
  return (*this);
}

CgiConnector::~CgiConnector() { this->env_.clear(); }

std::map<std::string, std::string> CgiConnector::getHeader() const {
  std::map<std::string, std::string> res;
  std::istringstream iss(this->respHeader_);
  std::string buffer;
  std::string key;
  std::string value;

  while (std::getline(iss, buffer)) {
    try {
      key = buffer.substr(0, buffer.find(":"));
      value = buffer.substr(buffer.find(":") + 2, buffer.length());
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
    res.insert(std::pair<std::string, std::string>(key, value));
  }
  return (res);
}

std::string CgiConnector::getBody() const { return this->respBody_; }

bool CgiConnector::checkCgi_() {
  std::string name = this->env_["SCRIPT_NAME"];
  std::string extention = name.substr(name.rfind(".") + 1, name.length());
  if (extention != "php" && extention != "py" && extention != "pl")
    return (false);

  struct dirent* file;
  DIR* bin = opendir("./cgi-bin");
  while ((file = readdir(bin)) != NULL)
    if (name == file->d_name) return (closedir(bin), true);
  closedir(bin);
  return (false);
}

envMap CgiConnector::buildEnv_(const Request& request) {
  envMap res;
  std::string uri = request.getPath();
  res.insert(envVar("REQUEST_METHOD", request.getRequestMethodString()));
  res.insert(envVar("QUERY_STRING", request.getQueryString()));
  res.insert(envVar("REQUEST_URI", uri));
  res.insert(envVar("SCRIPT_NAME", uri.substr(uri.rfind("/") + 1, uri.size())));
  res.insert(envVar("SERVER_PROTOCOL", request.getHTTPVersion()));
  try {
    res.insert(envVar("CONTENT_TYPE", request["Content-Type"]));
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  try {
    res.insert(envVar("CONTENT_LENGTH", request["Content-Length"]));
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return (res);
}

char** CgiConnector::envToString_() {
  size_t i = 0;
  std::string variable;
  char** res = new char*[this->env_.size() + 1];
  for (envMap::iterator it = env_.begin(); it != env_.end(); ++it) {
    variable = it->first + "=" + it->second;
    res[i] = new char[variable.length() + 1];
    strcpy(res[i], variable.c_str());
    res[i][variable.length()] = 0;
    ++i;
  }
  res[i] = NULL;
  return (res);
}

void CgiConnector::executeScript_(std::string path, InOutHandler& io) {
  io.dupInChild();
  char** env = this->envToString_();
  if (this->env_["REQUEST_METHOD"] == "POST") std::cout << this->reqBody_;
  execve(path.c_str(), NULL, env);
  // size_t i = 0;
  // while (env[i] != NULL) delete[] env[i];
  // delete[] env;
  std::exit(1);
}

void CgiConnector::readOutput_() {
  std::string buffer;
  while (std::getline(std::cin, buffer) && buffer != "\r\n")
    this->respHeader_.append(buffer);
  while (std::getline(std::cin, buffer)) this->respBody_.append(buffer);
}

std::string pathHelper(std::string name) {
  char* root = getenv("PWD");
  std::string res = std::string(root) + "/cgi-bin/" + name;
  return (res);
}

void CgiConnector::makeConnection(Status& status) {
  InOutHandler io;
  int exitcode;
  pid_t pid = fork();
  if (pid == 0) this->executeScript_(pathHelper(this->env_["SCRIPT_NAME"]), io);
  io.dupInParent();
  waitpid(pid, &exitcode, 0);
  if (exitcode > 1) status = 500;
  this->readOutput_();
}
