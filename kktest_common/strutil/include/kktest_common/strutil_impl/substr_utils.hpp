#ifndef KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_SUBSTR_UTILS_HPP_
#define KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_SUBSTR_UTILS_HPP_

#include <string>

namespace strutil {

bool startsWith(const std::string& str, const std::string& prefix);

bool startsWith(const std::string& str, char ch);

bool endsWith(const std::string& str, const std::string& suffix);

bool endsWith(const std::string& str, char ch);

bool containsSubstring(const std::string& str, const std::string& substr);

bool containsCharacter(const std::string& str, char ch);

}  // namespace strutil

#endif  // KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_SUBSTR_UTILS_HPP_
