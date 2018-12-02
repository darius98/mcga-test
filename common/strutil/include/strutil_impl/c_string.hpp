#ifndef COMMON_STRUTIL_INCLUDE_STRUTIL_IMPL_C_STRING_HPP_
#define COMMON_STRUTIL_INCLUDE_STRUTIL_IMPL_C_STRING_HPP_

#include <string>

namespace strutil {

char* copyAsCString(const char* source);

char* copyAsCString(const std::string& source);

}  // namespace strutil

#endif  // COMMON_STRUTIL_INCLUDE_STRUTIL_IMPL_C_STRING_HPP_
