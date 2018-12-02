#ifndef KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_CASING_HPP_
#define KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_CASING_HPP_

#include <string>

namespace kktest {
namespace strutil {

std::string toUpper(const std::string& str);

std::string toLower(const std::string& str);

std::string stripTrailingWhitespace(std::string str);

std::string strip(std::string str);

}  // namespace strutil
}  // namespace kktest

#endif  // KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_CASING_HPP_
