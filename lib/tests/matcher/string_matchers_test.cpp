#include <cctype>

#include <kktest>
#include <kktest_matchers>

using namespace kktest;
using namespace std;

void kkTestCase() {
    kkGroup("Specific charset matchers", []() {
        kkTest("isLetter matches only letters", []() {
            for (unsigned char i = 1; i <= 127; ++ i) {
                kkExpect(isLetter.matches(i) == (('a' <= i && i <= 'z')
                                                 || ('A' <= i && i <= 'Z')));
            }
        });

        kkTest("isDigit matches only digits", []() {
            for (unsigned char i = 1; i <= 127; ++ i) {
                kkExpect(isDigit.matches(i) == ('0' <= i && i <= '9'));
            }
        });


        kkTest("isLowercaseLetter matches only lowercase letters", []() {
            for (unsigned char i = 1; i <= 127; ++ i) {
                kkExpect(isLowercaseLetter.matches(i) == ('a' <= i && i <= 'z'));
            }
        });

        kkTest("isUppercaseLetter matches only uppercase letters", []() {
            for (unsigned char i = 1; i <= 127; ++ i) {
                kkExpect(isUppercaseLetter.matches(i) == ('A' <= i && i <= 'Z'));
            }
        });

        kkTest("isBinaryDigit matches only binary digits", []() {
            for (unsigned char i = 1; i <= 127; ++ i) {
                kkExpect(isBinaryDigit.matches(i) == ('0' <= i && i <= '1'));
            }
        });

        kkTest("isOctDigit matches only octal digits", []() {
            for (unsigned char i = 1; i <= 127; ++ i) {
                kkExpect(isOctDigit.matches(i) == ('0' <= i && i <= '7'));
            }
        });

        kkTest("isHexDigit matches only hexadecimal digits", []() {
            for (unsigned char i = 1; i <= 127; ++ i) {
                kkExpect(isHexDigit.matches(i) == (('0' <= i && i <= '9')
                                                   || ('a' <= i && i <= 'f')
                                                   || ('A' <= i && i <= 'F')));
            }
        });

        kkTest("isWhitespace matches only whitespace characters", []() {
            for (unsigned char i = 1; i <= 127; ++ i) {
                kkExpect(isWhitespace.matches(i) == (bool)isspace(i));
            }

        });
    });

    kkGroup("isSubstring", []() {
        kkTest("Empty string is substring of anything", []() {
            kkExpect(isSubstringOf("Empty").matches(""));
            kkExpect(isSubstringOf(" string i").matches(""));
            kkExpect(isSubstringOf("s a substring of anyth").matches(""));
            kkExpect(isSubstringOf("").matches(""));
            kkExpect(isSubstringOf("ing").matches(""));
        });

        kkTest("Nothing but the empty string is substring of the empty substring", []() {
            kkExpect(isSubstringOf("").matches(""));
            kkExpect(!isSubstringOf("").matches("Nothing "));
            kkExpect(!isSubstringOf("").matches("i"));
            kkExpect(!isSubstringOf("").matches("s"));
            kkExpect(!isSubstringOf("").matches(" a substring of the"));
            kkExpect(!isSubstringOf("").matches("empty string"));
        });

        kkTest("Any string is a substring of itself", []() {
            kkExpect(isSubstringOf("Any").matches("Any"));
            kkExpect(isSubstringOf(" string i").matches(" string i"));
            kkExpect(isSubstringOf("s a substring of itsel").matches("s a substring of itsel"));
            kkExpect(isSubstringOf("f").matches("f"));
        });

        kkTest("All substrings of 'Hello World!' match isSubstring('Hello World!')", []() {
            string s = "Hello World!";
            for (size_t start = 0; start < s.length(); ++ start) {
                for (size_t len = 0; start + len <= s.length(); ++ len) {
                    kkExpect(isSubstringOf(s).matches(s.substr(start, len)));
                }
            }
        });

        kkTest("'Hello' is not a substring of 'World!'", []() {
            kkExpect(!isSubstringOf("World!").matches("Hello"));
        });

        kkTest("'Hello' is not a substring of 'ello'", []() {
            kkExpect(!isSubstringOf("ello").matches("Hello"));
        });

        kkTest("'Hello' is not a substring of 'hello!!'", []() {
            kkExpect(!isSubstringOf("hello!!").matches("Hello"));
        });
    });
}
