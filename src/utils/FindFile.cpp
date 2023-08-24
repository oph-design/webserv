#include "FindFile.hpp"

bool findFile(std::string name, std::string dir) {
  struct dirent* file;
  DIR* bin = opendir(dir.c_str());
  while ((file = readdir(bin)) != NULL)
    if (name == file->d_name) return (closedir(bin), true);
  closedir(bin);
  return (false);
}
