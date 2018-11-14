#ifndef COMMON_STRUTIL_INCLUDE_STRUTIL_PUBLIC_SUBSTR_UTILS_H_
#define COMMON_STRUTIL_INCLUDE_STRUTIL_PUBLIC_SUBSTR_UTILS_H_

#include <string>

namespace strutil {

bool startsWith(const std::string& str, const std::string& prefix);

bool startsWith(const std::string& str, char ch);

bool endsWith(const std::string& str, const std::string& suffix);

bool endsWith(const std::string& str, char ch);

bool containsSubstring(const std::string& str, const std::string& substr);

bool containsCharacter(const std::string& str, char ch);

}

#endif
