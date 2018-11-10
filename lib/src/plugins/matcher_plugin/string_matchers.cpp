#include <cstring>

#include <matchers/string_matchers.hpp>


using namespace std;

namespace kktest {

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

CharInStringMatcher* isLetter = new CharInStringMatcher(
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "a letter");

CharInStringMatcher* isDigit  = new CharInStringMatcher("0123456789", "a digit");

CharInStringMatcher* isLowercaseLetter = new CharInStringMatcher(
        "abcdefghijklmnopqrstuvwxyz", "a lowercase letter");

CharInStringMatcher* isUppercaseLetter = new CharInStringMatcher(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "an uppercase letter");

CharInStringMatcher* isBinaryDigit = new CharInStringMatcher("01", "a binary digit");

CharInStringMatcher* isOctDigit = new CharInStringMatcher("01234567", "an oct digit");

CharInStringMatcher* isHexDigit = new CharInStringMatcher("0123456789ABCDEFabcdef", "a hex digit");

CharInStringMatcher* isWhitespace = new CharInStringMatcher(" \t\r\n", "a whitespace character");

IsSubstringMatcher::IsSubstringMatcher(string _container): container(move(_container)) {}

bool IsSubstringMatcher::matches(const string& object) {
    return container.find(object) != string::npos;
}

void IsSubstringMatcher::describe(Description& description) {
    description << "a substring of '" << container << "'";
}

void IsSubstringMatcher::describeMismatch(Description& description) {
    description << "not a substring of '" << container << "'";
}

IsSubstringMatcher* isSubstringOf(const std::string& s) {
    return new IsSubstringMatcher(s);
}

}
