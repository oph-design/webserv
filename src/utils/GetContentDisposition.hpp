#ifndef GETCONTENTDISPOSITION_HPP
#define GETCONTENTDISPOSITION_HPP

#include <exception>
#include <string>

#include "Request.hpp"
#include "Trim.hpp"

std::string getContentDisposition(const Request &request,
                                  const std::string &key);

#endif // GETCONTENTDISPOSITION_HPP
