#ifndef FINDFILE_HPP
#define FINDFILE_HPP

#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

bool findFile(std::string name, std::string dir);

#endif  // !FINDFILE_HPP
