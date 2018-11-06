#include <cctype>

#include <algorithm>

#include "string.hpp"

using namespace std;

namespace kktest_runner {

string toUpper(const string& str) {
    string ret(str.length(), '\0');
    transform(str.begin(), str.end(), ret.begin(), [](unsigned char c) {
        return toupper(c);
    });
    return ret;
}

string toLower(const string& str) {
    string ret(str.length(), '\0');
    transform(str.begin(), str.end(), ret.begin(), [](unsigned char c) {
        return tolower(c);
    });
    return ret;
}

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

string stripTrailingWhitespace(string str) {
    while (!str.empty() && isspace(str[str.length() - 1])) {
        str.erase(str.length() - 1);
    }
    return str;
}

string strip(string str) {
    str = stripTrailingWhitespace(str);
    reverse(str.begin(), str.end());
    str = stripTrailingWhitespace(str);
    reverse(str.begin(), str.end());
    return str;
}

}
