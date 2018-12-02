#ifndef KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_C_STRING_HPP_
#define KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_C_STRING_HPP_

#include <string>

namespace strutil {

char* copyAsCString(const char* source);

char* copyAsCString(const std::string& source);

}  // namespace strutil

#endif  // KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_C_STRING_HPP_
