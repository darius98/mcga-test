#include <algorithm>

#include <strutil_impl/casing.hpp>

using namespace std;

namespace strutil {

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
