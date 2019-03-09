#include <cctype>

#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers/string.hpp"
#include "kktest/extensions/matchers/include/kktest_ext/matchers/testing_helpers.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace std;

void stringMatchersTest() {
    group("Specific charset matchers", [] {
        test("isLetter matches only letters", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                if (('a' <= i && i <= 'z') || ('A' <= i && i <= 'Z')) {
                    EXPECT_MATCHER_MATCHES(i, isLetter);
                } else {
                    EXPECT_MATCHER_FAILS(i, isLetter);
                }
            }
        });

        test("isDigit matches only digits", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                if ('0' <= i && i <= '9') {
                    EXPECT_MATCHER_MATCHES(i, isDigit);
                } else {
                    EXPECT_MATCHER_FAILS(i, isDigit);
                }
            }
        });


        test("isLowercaseLetter matches only lowercase letters", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                if ('a' <= i && i <= 'z') {
                    EXPECT_MATCHER_MATCHES(i, isLowercaseLetter);
                } else {
                    EXPECT_MATCHER_FAILS(i, isLowercaseLetter);
                }
            }
        });

        test("isUppercaseLetter matches only uppercase letters", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                if ('A' <= i && i <= 'Z') {
                    EXPECT_MATCHER_MATCHES(i, isUppercaseLetter);
                } else {
                    EXPECT_MATCHER_FAILS(i, isUppercaseLetter);
                }
            }
        });

        test("isBinaryDigit matches only binary digits", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                if ('0' <= i && i <= '1') {
                    EXPECT_MATCHER_MATCHES(i, isBinaryDigit);
                } else {
                    EXPECT_MATCHER_FAILS(i, isBinaryDigit);
                }
            }
        });

        test("isOctDigit matches only octal digits", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                if ('0' <= i && i <= '7') {
                    EXPECT_MATCHER_MATCHES(i, isOctDigit);
                } else {
                    EXPECT_MATCHER_FAILS(i, isOctDigit);
                }
            }
        });

        test("isHexDigit matches only hexadecimal digits", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                if (('0' <= i && i <= '9')
                        || ('a' <= i && i <= 'f')
                        || ('A' <= i && i <= 'F')) {
                    EXPECT_MATCHER_MATCHES(i, isHexDigit);
                } else {
                    EXPECT_MATCHER_FAILS(i, isHexDigit);
                }
            }
        });

        test("isWhitespace matches only whitespace characters", [] {
            for (unsigned char i = 1; i <= 127; ++i) {
                if (static_cast<bool>(isspace(i))) {
                    EXPECT_MATCHER_MATCHES(i, isWhitespace);
                } else {
                    EXPECT_MATCHER_FAILS(i, isWhitespace);
                }
            }
        });
    });

    group("isSubstring", [] {
        test("Empty string is substring of anything", [] {
            EXPECT_MATCHER_MATCHES("", isSubstringOf("Empty"));
            EXPECT_MATCHER_MATCHES("", isSubstringOf(" string i"));
            EXPECT_MATCHER_MATCHES("", isSubstringOf("s a substring of anyth"));
            EXPECT_MATCHER_MATCHES("", isSubstringOf(""));
            EXPECT_MATCHER_MATCHES("", isSubstringOf("ing"));
        });

        test("Nothing but the empty string is substring of the empty substring",
             [] {
            EXPECT_MATCHER_MATCHES("", isSubstringOf(""));
            EXPECT_MATCHER_FAILS("Nothing ", isSubstringOf(""));
            EXPECT_MATCHER_FAILS("i", isSubstringOf(""));
            EXPECT_MATCHER_FAILS("s", isSubstringOf(""));
            EXPECT_MATCHER_FAILS(" a substring of the", isSubstringOf(""));
            EXPECT_MATCHER_FAILS("empty string", isSubstringOf(""));
        });

        test("Any string is a substring of itself", [] {
            EXPECT_MATCHER_MATCHES("Any", isSubstringOf("Any"));
            EXPECT_MATCHER_MATCHES(" string i", isSubstringOf(" string i"));
            EXPECT_MATCHER_MATCHES("s a substring of itsel",
                                   isSubstringOf("s a substring of itsel"));
            EXPECT_MATCHER_MATCHES("f", isSubstringOf("f"));
        });

        test("All substrings of 'Hello World!' match "
             "isSubstring('Hello World!')", [] {
            string s = "Hello World!";
            for (size_t start = 0; start < s.length(); ++start) {
                for (size_t len = 0; start + len <= s.length(); ++len) {
                    EXPECT_MATCHER_MATCHES(s.substr(start, len),
                                           isSubstringOf(s));
                }
            }
        });

        test("'Hello' is not a substring of 'World!'", [] {
            EXPECT_MATCHER_FAILS("Hello", isSubstringOf("World!"));
        });

        test("'Hello' is not a substring of 'ello'", [] {
            EXPECT_MATCHER_FAILS("Hello", isSubstringOf("ello"));
        });

        test("'Hello' is not a substring of 'hello!!'", [] {
            EXPECT_MATCHER_FAILS("Hello", isSubstringOf("hello!!"));
        });
    });
}
