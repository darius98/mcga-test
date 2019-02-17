#include "common/string/src/string.hpp"

#include <cstring>

#include <algorithm>

using std::string;
using std::transform;

namespace kktest {

String::String() = default;

String::String(string::size_type sz, char ch): string(sz, ch) {}

String::String(const string& s): string(s) {}

String::String(const char* s): string(s) {}

String String::toUpper() const {
    String ret(length(), '\0');
    transform(begin(), end(), ret.begin(), toupper);
    return ret;
}

String String::toLower() const {
    String ret(length(), '\0');
    transform(begin(), end(), ret.begin(), tolower);
    return ret;
}

String String::stripTrailingWhitespace() const {
    String str = *this;
    while (!str.empty() && isspace(str[str.length() - 1])) {
        str.erase(str.length() - 1);
    }
    return str;
}

String String::strip() const {
    String str = stripTrailingWhitespace();
    reverse(str.begin(), str.end());
    str = str.stripTrailingWhitespace();
    reverse(str.begin(), str.end());
    return str;
}

string String::str() const {
    return string(begin(), end());
}


bool String::startsWith(const String& prefix) {
    if (prefix.length() > length()) {
        return false;
    }
    return substr(0, prefix.length()) == prefix;
}

bool String::startsWith(char ch) {
    return !empty() && at(0) == ch;
}

bool String::endsWith(const String& suffix) {
    if (suffix.length() > length()) {
        return false;
    }
    return substr(length() - suffix.length()) == suffix;
}

bool String::endsWith(char ch) {
    return !empty() && at(length() - 1) == ch;
}

bool String::containsSubstring(const String& substr) {
    return find(substr) != npos;
}

bool String::containsCharacter(char ch) {
    return find(ch) != npos;
}

char* String::copyAsCStr() const {
    auto size = (length() + 1) * sizeof(char);
    auto destination = malloc(size);
    memcpy(destination, c_str(), size);
    return static_cast<char*>(destination);
}

}
