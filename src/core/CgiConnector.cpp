#include "CgiConnector.hpp"

#include "colors.hpp"

CgiConnector::CgiConnector() {}

CgiConnector::CgiConnector(const Request& request) {
  if (request.getRequestBodyExists()) this->reqBody_ = request.getRequestBody();
  this->isCgi = checkCgi_(request.getPath());
  if (this->isCgi) this->env_ = buildEnv_(request);
}

CgiConnector::CgiConnector(const CgiConnector& rhs) { *this = rhs; }

CgiConnector& CgiConnector::operator=(const CgiConnector& rhs) {
  this->respBody_ = rhs.respBody_;
  this->respHeader_ = rhs.respHeader_;
  this->env_ = rhs.env_;
  this->isCgi = rhs.isCgi;
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

bool CgiConnector::checkCgi_(std::string uri) {
  std::string name = uri.substr(uri.rfind("/") + 1, uri.size());
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

void CgiConnector::executeScript_(std::string path, int pipes[2]) {
  dup2(pipes[1], 1);
  close(pipes[0]);
  close(pipes[1]);
  char** env = this->envToString_();
  char** args = new char*;
  *args = const_cast<char*>(path.c_str());
  if (this->env_["REQUEST_METHOD"] == "POST") std::cout << this->reqBody_;
  execve(path.c_str(), args, env);
  size_t i = 0;
  while (env[i] != NULL) delete env[i++];
  delete[] env;
  delete args;
  std::exit(1);
}

void CgiConnector::readOutput_(int pipes[2]) {
  int in = dup(0);
  dup2(pipes[0], 0);
  close(pipes[0]);
  close(pipes[1]);
  std::string buffer;
  while (std::getline(std::cin, buffer) && buffer.length() != 0)
    this->respHeader_.append(buffer + "\r\n");
  while (std::getline(std::cin, buffer)) this->respBody_.append(buffer);
  dup2(in, 0);
  close(in);
}

std::string pathHelper(std::string name) {
  char* root = getenv("PWD");
  std::string res = std::string(root) + "/cgi-bin/" + name;
  return (res);
}

pid_t CgiConnector::waitAny(int* exitcode) {
  pid_t pid = 0;
  while ((pid = waitpid(WAIT_ANY, exitcode, 0)) == -1) {
    if (errno == EINTR) {
      continue;
    } else {
      perror("waitpid");
      abort();
    }
  }
  return pid;
}

void CgiConnector::makeConnection(Status& status) {
  int pipes[2];
  int exitcode;
  pipe(pipes);
  pid_t timer = fork();
  pid_t pid = fork();
  if (timer == 0) {
    sleep(1);
    std::exit(112);
  }
  if (!pid) this->executeScript_(pathHelper(this->env_["SCRIPT_NAME"]), pipes);
  if (waitAny(&exitcode) == timer)
    kill(pid, SIGKILL);
  else
    kill(timer, SIGKILL);
  if (exitcode > 0) return (void)(status = 500);
  this->readOutput_(pipes);
}
