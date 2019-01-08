#ifndef KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_KKTEST_STRING_HPP_
#define KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_KKTEST_STRING_HPP_

#include <string>

namespace kktest {

class String: public std::string {
 public:
    String();
    String(std::string::size_type sz, char ch);
    String(const std::string& s);
    String(const char* s);

    String toUpper() const;
    String toLower() const;

    String stripTrailingWhitespace() const;
    String strip() const;

    std::string str() const;

    bool startsWith(const String& prefix);

    bool startsWith(char ch);

    bool endsWith(const String& suffix);

    bool endsWith(char ch);

    bool containsSubstring(const String& substr);

    bool containsCharacter(char ch);
};

}

#endif  // KKTEST_COMMON_STRUTIL_INCLUDE_KKTEST_COMMON_STRUTIL_IMPL_KKTEST_STRING_HPP_
