#ifndef GETCONTENTDISPOSITION_HPP
#define GETCONTENTDISPOSITION_HPP

#include <exception>
#include <string>

#include "Request.hpp"
#include "Trim.hpp"

std::string getContentDispostion(const Request& request, std::string key);

#endif  // !GETCONTENTDISPOSITION_HPP
