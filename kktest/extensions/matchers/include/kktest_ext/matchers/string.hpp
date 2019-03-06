#pragma once

#include <string>

#include <kktest_ext/matchers/matcher.hpp>
#include <kktest_ext/matchers/detail/decl.hpp>

namespace kktest::matchers {

namespace detail {

class CharInStringMatcher: public Matcher {
 public:
    explicit CharInStringMatcher(const char* container,
                                 const char* expectation = nullptr) noexcept:
            container(container), expectation(expectation) {}

    bool matches(const char& ch) {
        return strchr(container, ch) != nullptr;
    }

    void describe(Description* description) {
        if (expectation != nullptr) {
            (*description) << expectation;
        } else {
            (*description) << "character in '" << container << "'";
        }
    }

    void describeMismatch(Description* description, const char& ch) {
        if (expectation != nullptr) {
            (*description) << "not " << expectation;
        } else {
            (*description) << "character that is not in '" << container << "'";
        }
    }

 private:
    const char* container;
    const char* expectation;
};

class IsSubstringMatcher: public Matcher {
 public:
    explicit IsSubstringMatcher(std::string container):
            container(std::move(container)) {}

    bool matches(const std::string& object) {
        return container.find(object) != std::string::npos;
    }

    void describe(Description* description) {
        (*description) << "a substring of '" << container << "'";
    }

    void describeMismatch(Description* description, const std::string& object) {
        (*description) << "not a substring";
    }

 private:
    std::string container;
};

}

static detail::CharInStringMatcher isLetter(
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "a letter");

static detail::CharInStringMatcher isDigit(
        "0123456789", "a digit");

static detail::CharInStringMatcher isLowercaseLetter(
        "abcdefghijklmnopqrstuvwxyz", "a lowercase letter");

static detail::CharInStringMatcher isUppercaseLetter(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "an uppercase letter");

static detail::CharInStringMatcher isBinaryDigit(
        "01", "a binary digit");

static detail::CharInStringMatcher isOctDigit(
        "01234567", "an oct digit");

static detail::CharInStringMatcher isHexDigit(
        "0123456789ABCDEFabcdef", "a hex digit");

static detail::CharInStringMatcher isWhitespace(
        " \t\r\n\f\v", "a whitespace character");

static detail::IsSubstringMatcher isSubstringOf(const std::string& s) {
    return detail::IsSubstringMatcher(s);
}

}
