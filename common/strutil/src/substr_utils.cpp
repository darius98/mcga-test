#include <strutil_impl/substr_utils.hpp>

using std::string;

namespace strutil {

bool startsWith(const string& str, const string& prefix) {
    if (prefix.length() > str.length()) {
        return false;
    }
    return str.substr(0, prefix.length()) == prefix;
}

bool startsWith(const string& str, char ch) {
    return !str.empty() && str[0] == ch;
}

bool endsWith(const string& str, const string& suffix) {
    if (suffix.length() > str.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix;
}

bool endsWith(const string& str, char ch) {
    return !str.empty() && str[str.length() - 1] == ch;
}

bool containsSubstring(const string& str, const string& substr) {
    return str.find(substr) != string::npos;
}

bool containsCharacter(const string& str, char ch) {
    return str.find(ch) != string::npos;
}

}  // namespace strutil
