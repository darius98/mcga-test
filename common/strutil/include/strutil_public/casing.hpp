#ifndef COMMON_STRUTIL_INCLUDE_STRUTIL_PUBLIC_CASING_H_
#define COMMON_STRUTIL_INCLUDE_STRUTIL_PUBLIC_CASING_H_

#include <string>

namespace strutil {

std::string toUpper(const std::string& str);

std::string toLower(const std::string& str);

std::string stripTrailingWhitespace(std::string str);

std::string strip(std::string str);

}

#endif
