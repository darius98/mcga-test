#include "core_matchers.hpp"

using namespace std;


namespace kktest {

IsTrueMatcher* isTrue = new IsTrueMatcher();

IsFalseMatcher* isFalse = new IsFalseMatcher();

IsEmptyMatcher* isEmpty = new IsEmptyMatcher();

IsNotEmptyMatcher* isNotEmpty = new IsNotEmptyMatcher();

IsNullptrMatcher* isNull = new IsNullptrMatcher();

IsNotNullptrMatcher* isNotNull = new IsNotNullptrMatcher();

ThrowsAnythingMatcher* throws = new ThrowsAnythingMatcher();

IsPositiveMatcher* isPositive = new IsPositiveMatcher();

IsNegativeMatcher* isNegative = new IsNegativeMatcher();

IsEvenMatcher* isEven = new IsEvenMatcher();

IsOddMatcher* isOdd = new IsOddMatcher();

IsZeroMatcher* isZero = new IsZeroMatcher();

CharInStringMatcher* isLetter = new CharInStringMatcher(
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "a letter");

CharInStringMatcher* isDigit  = new CharInStringMatcher(
        "0123456789", "a digit");

CharInStringMatcher* isLowercaseLetter = new CharInStringMatcher(
        "abcdefghijklmnopqrstuvwxyz", "a lowercase letter");

CharInStringMatcher* isUppercaseLetter = new CharInStringMatcher(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "an uppercase letter");

CharInStringMatcher* isBinaryDigit = new CharInStringMatcher(
        "01", "a binary digit");

CharInStringMatcher* isOctDigit = new CharInStringMatcher(
        "01234567", "an oct digit");

CharInStringMatcher* isHexDigit = new CharInStringMatcher(
        "0123456789ABCDEFabcdef", "a hex digit");

CharInStringMatcher* isWhitespace = new CharInStringMatcher(
        " \t\r\n", "a whitespace character");

IsSubstringMatcher* isSubstringOf(const std::string& s) {
    return new IsSubstringMatcher(s);
}

}
