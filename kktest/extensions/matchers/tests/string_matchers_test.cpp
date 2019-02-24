#include <cctype>

#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers_impl/string_matchers.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace std;

void stringMatchersTest() {
    group("Specific charset matchers", [] {
        test("isLetter matches only letters", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                expect(isLetter.matches(i) == (('a' <= i && i <= 'z') ||
                                               ('A' <= i && i <= 'Z')));
            }
        });

        test("isDigit matches only digits", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                expect(isDigit.matches(i) == ('0' <= i && i <= '9'));
            }
        });


        test("isLowercaseLetter matches only lowercase letters", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                expect(isLowercaseLetter.matches(i) == ('a' <= i && i <= 'z'));
            }
        });

        test("isUppercaseLetter matches only uppercase letters", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                expect(isUppercaseLetter.matches(i) == ('A' <= i && i <= 'Z'));
            }
        });

        test("isBinaryDigit matches only binary digits", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                expect(isBinaryDigit.matches(i) == ('0' <= i && i <= '1'));
            }
        });

        test("isOctDigit matches only octal digits", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                expect(isOctDigit.matches(i) == ('0' <= i && i <= '7'));
            }
        });

        test("isHexDigit matches only hexadecimal digits", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                expect(isHexDigit.matches(i) == (('0' <= i && i <= '9')
                                                   || ('a' <= i && i <= 'f')
                                                   || ('A' <= i && i <= 'F')));
            }
        });

        test("isWhitespace matches only whitespace characters", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                expect(
                    isWhitespace.matches(i) == static_cast<bool>(isspace(i)));
            }
        });
    });

    group("isSubstring", [] {
        test("Empty string is substring of anything", [] {
            expect(isSubstringOf("Empty").matches(""));
            expect(isSubstringOf(" string i").matches(""));
            expect(isSubstringOf("s a substring of anyth").matches(""));
            expect(isSubstringOf("").matches(""));
            expect(isSubstringOf("ing").matches(""));
        });

        test("Nothing but the empty string is substring of the empty substring",
             [] {
            expect(isSubstringOf("").matches(""));
            expect(!isSubstringOf("").matches("Nothing "));
            expect(!isSubstringOf("").matches("i"));
            expect(!isSubstringOf("").matches("s"));
            expect(!isSubstringOf("").matches(" a substring of the"));
            expect(!isSubstringOf("").matches("empty string"));
        });

        test("Any string is a substring of itself", [] {
            expect(isSubstringOf("Any").matches("Any"));
            expect(isSubstringOf(" string i").matches(" string i"));
            expect(isSubstringOf("s a substring of itsel")
                       .matches("s a substring of itsel"));
            expect(isSubstringOf("f").matches("f"));
        });

        test("All substrings of 'Hello World!' match "
             "isSubstring('Hello World!')", [] {
            string s = "Hello World!";
            for (size_t start = 0; start < s.length(); ++start) {
                for (size_t len = 0; start + len <= s.length(); ++len) {
                    expect(isSubstringOf(s).matches(s.substr(start, len)));
                }
            }
        });

        test("'Hello' is not a substring of 'World!'", [] {
            expect(!isSubstringOf("World!").matches("Hello"));
        });

        test("'Hello' is not a substring of 'ello'", [] {
            expect(!isSubstringOf("ello").matches("Hello"));
        });

        test("'Hello' is not a substring of 'hello!!'", [] {
            expect(!isSubstringOf("hello!!").matches("Hello"));
        });
    });
}
