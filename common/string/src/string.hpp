#ifndef KKTEST_COMMON_STRING_SRC_STRING_HPP_
#define KKTEST_COMMON_STRING_SRC_STRING_HPP_

#include <string>

namespace kktest {

class String: public std::string {
 public:
    String();
    String(std::string::size_type sz, char ch);
    String(const std::string& s); // NOLINT(google-explicit-constructor)
    String(const char* s); // NOLINT(google-explicit-constructor)

    String toUpper() const;
    String toLower() const;

    String stripTrailingWhitespace() const;
    String strip() const;

    std::string str() const;
    char* copyAsCStr() const;

    bool startsWith(const String& prefix);

    bool startsWith(char ch);

    bool endsWith(const String& suffix);

    bool endsWith(char ch);

    bool containsSubstring(const String& substr);

    bool containsCharacter(char ch);
};

}

#endif
