#ifndef LAST_HPP_
#define LAST_HPP_

#include <string>

char last(const std::string& str) {
    if (str.empty())
        return 0;
    else
        return *(str.end() - 1);
}

#endif