#include <cstring>

#include <kktest_ext/matchers_impl/string_matchers.hpp>

using namespace std;

namespace kktest {
namespace matchers {

detail::CharInStringMatcher isLetter(
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "a letter");

detail::CharInStringMatcher isDigit("0123456789", "a digit");

detail::CharInStringMatcher isLowercaseLetter("abcdefghijklmnopqrstuvwxyz",
                                      "a lowercase letter");

detail::CharInStringMatcher isUppercaseLetter("ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                      "an uppercase letter");

detail::CharInStringMatcher isBinaryDigit("01", "a binary digit");

detail::CharInStringMatcher isOctDigit("01234567", "an oct digit");

detail::CharInStringMatcher isHexDigit("0123456789ABCDEFabcdef", "a hex digit");

detail::CharInStringMatcher isWhitespace(" \t\r\n\f\v",
                                         "a whitespace character");

detail::IsSubstringMatcher isSubstringOf(const string& s) {
    return detail::IsSubstringMatcher(s);
}

namespace detail {

CharInStringMatcher::CharInStringMatcher(const char* _container,
                                         const char* _expectation) noexcept:
        container(_container), expectation(_expectation) {}

bool CharInStringMatcher::matches(const char& ch) {
    return strchr(container, ch) != nullptr;
}

void CharInStringMatcher::describe(Description* description) {
    if (expectation != nullptr) {
        (*description) << expectation;
    } else {
        (*description) << "character in '" << container << "'";
    }
}

void CharInStringMatcher::describeMismatch(Description* description,
                                           const char& ch) {
    if (expectation != nullptr) {
        (*description) << "not " << expectation;
    } else {
        (*description) << "character that is not in '" << container << "'";
    }
}

IsSubstringMatcher::IsSubstringMatcher(string _container):
        container(move(_container)) {}

bool IsSubstringMatcher::matches(const string& object) {
    return container.find(object) != string::npos;
}

void IsSubstringMatcher::describe(Description* description) {
    (*description) << "a substring of '" << container << "'";
}

void IsSubstringMatcher::describeMismatch(Description* description,
                                          const string&) {
    (*description) << "not a substring";
}

}
}
}
