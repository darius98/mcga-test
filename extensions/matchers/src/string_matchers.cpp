#include "extensions/matchers/include/kktest_ext/matchers_impl/string_matchers.hpp"

#include <cstring>

#include "core/src/export.hpp"

using namespace std;

namespace kktest {
namespace matchers {

KKTEST_EXPORT detail::CharInStringMatcher isLetter(
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "a letter");

KKTEST_EXPORT detail::CharInStringMatcher isDigit("0123456789", "a digit");

KKTEST_EXPORT detail::CharInStringMatcher isLowercaseLetter(
        "abcdefghijklmnopqrstuvwxyz", "a lowercase letter");

KKTEST_EXPORT detail::CharInStringMatcher isUppercaseLetter(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "an uppercase letter");

KKTEST_EXPORT detail::CharInStringMatcher isBinaryDigit("01", "a binary digit");

KKTEST_EXPORT detail::CharInStringMatcher isOctDigit(
        "01234567", "an oct digit");

KKTEST_EXPORT detail::CharInStringMatcher isHexDigit(
        "0123456789ABCDEFabcdef", "a hex digit");

KKTEST_EXPORT detail::CharInStringMatcher isWhitespace(
        " \t\r\n\f\v", "a whitespace character");

KKTEST_EXPORT detail::IsSubstringMatcher isSubstringOf(const string& s) {
    return detail::IsSubstringMatcher(s);
}

namespace detail {

KKTEST_EXPORT CharInStringMatcher::CharInStringMatcher(
        const char* _container, const char* _expectation) noexcept:
    container(_container), expectation(_expectation) {}

KKTEST_EXPORT bool CharInStringMatcher::matches(const char& ch) {
    return strchr(container, ch) != nullptr;
}

KKTEST_EXPORT void CharInStringMatcher::describe(Description* description) {
    if (expectation != nullptr) {
        (*description) << expectation;
    } else {
        (*description) << "character in '" << container << "'";
    }
}

KKTEST_EXPORT void CharInStringMatcher::describeMismatch(
        Description* description, const char& ch) {
    if (expectation != nullptr) {
        (*description) << "not " << expectation;
    } else {
        (*description) << "character that is not in '" << container << "'";
    }
}

KKTEST_EXPORT IsSubstringMatcher::IsSubstringMatcher(string _container):
        container(move(_container)) {}

KKTEST_EXPORT bool IsSubstringMatcher::matches(const string& object) {
    return container.find(object) != string::npos;
}

KKTEST_EXPORT void IsSubstringMatcher::describe(Description* description) {
    (*description) << "a substring of '" << container << "'";
}

KKTEST_EXPORT void IsSubstringMatcher::describeMismatch(
        Description* description, const string&) {
    (*description) << "not a substring";
}

}
}
}
