#pragma once

#include "mcga/test_ext/matchers/detail/string.hpp"

namespace mcga::test::matchers {

constexpr detail::CharInStringMatcher
  isLetter("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "a letter");

constexpr detail::CharInStringMatcher isDigit("0123456789", "a digit");

constexpr detail::CharInStringMatcher
  isLowercaseLetter("abcdefghijklmnopqrstuvwxyz", "a lowercase letter");

constexpr detail::CharInStringMatcher
  isUppercaseLetter("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "an uppercase letter");

constexpr detail::CharInStringMatcher isBinaryDigit("01", "a binary digit");

constexpr detail::CharInStringMatcher isOctDigit("01234567", "an oct digit");

constexpr detail::CharInStringMatcher isHexDigit("0123456789ABCDEFabcdef",
                                                 "a hex digit");

constexpr detail::CharInStringMatcher isWhitespace(" \t\r\n\f\v",
                                                   "a whitespace character");

inline detail::IsSubstringMatcher isSubstringOf(const std::string& s) {
    return detail::IsSubstringMatcher(s);
}

}  // namespace mcga::test::matchers
