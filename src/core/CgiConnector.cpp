#include "CgiConnector.hpp"

CgiConnector::CgiConnector() {}

CgiConnector::CgiConnector(const Request& request)
    : isCgi(checkCgi()),
      reqBody_(request.getRequestBody()),
      env_(buildEnv(request)) {}

CgiConnector::CgiConnector(const CgiConnector& rhs) { *this = rhs; }

CgiConnector& CgiConnector::operator=(const CgiConnector& rhs) {
  this->respBody_ = rhs.respBody_;
  this->respHeader_ = rhs.respHeader_;
  this->env_ = rhs.env_;
  return (*this);
}

CgiConnector::~CgiConnector() { this->env_.clear(); }

bool CgiConnector::checkCgi() {
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

envMap CgiConnector::buildEnv(const Request& request) {
  envMap res;
  std::string uri = request.getPath();
  res.insert(envVar("REQUEST_METHOD", request.getRequestMethodString()));
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
  res.insert(envVar("QUERY_STRING", request.getQueryString()));
  res.insert(envVar("REQUEST_URI", uri));
  res.insert(
      envVar("SCRIPT_NAME", uri.substr(uri.rfind("/") + 1, uri.length())));
  res.insert(envVar("SERVER_PROTOCOL", request.getHTTPVersion()));
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

void CgiConnector::deleteEnv_(char** env) {
  size_t i = 0;
  while (env[i] != NULL) delete[] env[i];
  delete[] env;
}

void CgiConnector::executeScript_(std::string path, InOutHandler& io) {
  io.dupInChild();
  char** env = this->envToString_();
  if (this->env_["REQUEST_METHOD"] == "POST") std::cout << this->reqBody_;
  execve(path.c_str(), NULL, env);
  deleteEnv_(env);
  std::exit(500);
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
  delete root;
  return (res);
}

int CgiConnector::makeConnection() {
  InOutHandler io;
  char** env = this->envToString_();
  if (fork() == 0)
    this->executeScript_(env, pathHelper(this->env_["SCRIPT_NAME"]), io);
  io.dupInParent();
  this->readOutput_();
  deleteEnv_(env);
  return (200);
}
