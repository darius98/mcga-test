#include <kktest_common/strutil_impl/unescape.hpp>

using std::string;

namespace strutil {

string unescapeCharacters(const string& s) {
    string unescaped;
    string::const_iterator it = s.begin();
    while (it != s.end()) {
        char ch = *it;
        ++it;
        if (ch == '\\' && it != s.end()) {
            char ch2 = *it;
            ++it;
            switch (ch2) {
                case '\\': ch = '\\'; break;
                case 'n': ch = '\n'; break;
                case 't': ch = '\t'; break;
                case '\'': ch = '\''; break;
                default:
                    unescaped += ch;
                    ch = ch2;
            }
        }
        unescaped += ch;
    }
    return unescaped;
}

}  // namespace strutil
