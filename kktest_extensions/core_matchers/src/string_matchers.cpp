#include <cstring>

#include <kktest_ext/core_matchers_impl/string_matchers.hpp>

using std::string;

namespace kktest {
namespace core_matchers {

CharInStringMatcher::CharInStringMatcher(const char* _container, const char* _expectation) noexcept:
        container(_container), expectation(_expectation) {}

bool CharInStringMatcher::matches(const char& ch) {
    return strchr(container, ch) != nullptr;
}

void CharInStringMatcher::describe(Description& description) {
    if (expectation != nullptr) {
        description << expectation;
    } else {
        description << "character in '" << container << "'";
    }
}

void CharInStringMatcher::describeMismatch(Description& description) {
    if (expectation != nullptr) {
        description << "not " << expectation;
    } else {
        description << "character that is not in '" << container << "'";
    }
}

CharInStringMatcher isLetter("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "a letter");

CharInStringMatcher isDigit("0123456789", "a digit");

CharInStringMatcher isLowercaseLetter("abcdefghijklmnopqrstuvwxyz", "a lowercase letter");

CharInStringMatcher isUppercaseLetter("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "an uppercase letter");

CharInStringMatcher isBinaryDigit("01", "a binary digit");

CharInStringMatcher isOctDigit("01234567", "an oct digit");

CharInStringMatcher isHexDigit("0123456789ABCDEFabcdef", "a hex digit");

CharInStringMatcher isWhitespace(" \t\r\n\f\v", "a whitespace character");

IsSubstringMatcher::IsSubstringMatcher(string _container): container(move(_container)) {}

bool IsSubstringMatcher::matches(const string& object) {
    return container.find(object) != string::npos;
}

void IsSubstringMatcher::describe(Description& description) {
    description << "a substring of '" << container << "'";
}

IsSubstringMatcher isSubstringOf(const string& s) {
    return IsSubstringMatcher(s);
}

}  // namespace core_matchers
}  // namespace kktest
