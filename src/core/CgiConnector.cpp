#include "CgiConnector.hpp"

#include <strings.h>
#include <sys/signal.h>
#include <sys/wait.h>

#include <csignal>

#include "colors.hpp"

CgiConnector::CgiConnector() {}

CgiConnector::CgiConnector(const Request& request)
    : reqBody_(request.getRequestBody()), env_(buildEnv_(request)) {}

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
      continue;
    }
    res.insert(std::pair<std::string, std::string>(key, value));
  }
  return (res);
}

std::string CgiConnector::getBody() const { return this->respBody_; }

bool CgiConnector::isCgi(std::string uri) {
  std::string name = uri.substr(uri.rfind("/") + 1, uri.size());
  std::string extention = name.substr(name.rfind(".") + 1, name.length());
  if (extention != "py") return (false);
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

std::string pathHelper(std::string name) {
  char* root = getenv("PWD");
  std::string res = std::string(root) + "/cgi-bin/" + name;
  return (res);
}

pid_t CgiConnector::waitAny(int* exitcode) {
  pid_t pid = 0;
  while (pid == 0) {
    pid = waitpid(WAIT_ANY, exitcode, WNOHANG);
  }
  return pid;
}

void CgiConnector::readOutput_(int pipes[2]) {
  close(pipes[1]);
  char buf[16];
  bzero(buf, 16);
  std::string bufferStr;
  while (read(pipes[0], buf, 15) > 0) {
    bufferStr.append(buf);
    bzero(buf, 16);
  }
  this->respHeader_ = bufferStr.substr(0, bufferStr.find("\n\n"));
  std::cout << this->respHeader_ << std::endl;
  this->respBody_ =
      bufferStr.substr(bufferStr.find("\n\n") + 2, bufferStr.size());
  std::cout << this->respBody_ << std::endl;
  close(pipes[0]);
}

void CgiConnector::writeReqBody() {
  int script_input[2];
  pipe(script_input);
  dup2(script_input[0], 0);
  close(script_input[0]);
  write(script_input[1], this->reqBody_.c_str(), this->reqBody_.size());
  close(script_input[1]);
}

void CgiConnector::executeScript_(std::string path, int pipes[2]) {
  if (this->env_["REQUEST_METHOD"] == "POST") this->writeReqBody();
  dup2(pipes[1], 1);
  close(pipes[0]);
  close(pipes[1]);
  char** env = this->envToString_();
  char** args = new char*[2];
  args[0] = const_cast<char*>(path.c_str());
  args[1] = NULL;
  execve(path.c_str(), args, env);
  size_t i = 0;
  while (env[i] != NULL) delete env[i++];
  delete[] env;
  delete[] args;
  std::exit(1);
}

void CgiConnector::makeConnection(Status& status) {
  int pipes[2];
  int exitcode;
  pid_t timer = fork();
  if (timer == 0) {
    sleep(100);
    std::exit(112);
  }
  pipe(pipes);
  pid_t pid = fork();
  if (!pid) this->executeScript_(pathHelper(this->env_["SCRIPT_NAME"]), pipes);
  if (waitAny(&exitcode) == timer)
    kill(pid, SIGTERM);
  else
    kill(timer, SIGTERM);
  waitpid(WAIT_ANY, NULL, 0);
  exitcode = WEXITSTATUS(exitcode);
  std::cout << RED << exitcode << COLOR_RESET << std::endl;
  if (exitcode > 0) {
    close(pipes[0]);
    close(pipes[1]);
    status = 500;
    return;
  }
  this->readOutput_(pipes);
}
