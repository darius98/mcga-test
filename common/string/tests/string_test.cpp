#include <cstring>

#include <kktest.hpp>
#include <kktest_ext/matchers.hpp>

#include "common/string/src/string.hpp"

using namespace kktest;
using namespace kktest::matchers;

void kkTestCase(KKTestString) {
    group("toLower", [] {
        test("toLower does not modify string with no letters", [] {
            expect(String("12345!@#$").toLower(),
                   isEqualTo(String("12345!@#$")));
        });

        test("toLower does not modify string with only lowercase letters", [] {
            expect(String("abcdef").toLower(), isEqualTo(String("abcdef")));
        });

        test("toLower modifies only uppercase letters", [] {
            expect(String("12AAbb##").toLower(), isEqualTo(String("12aabb##")));
        });
    });

    group("toUpper", [] {
        test("toUpper does not modify string with no letters", [] {
            expect(String("12345!@$").toUpper(), isEqualTo(String("12345!@$")));
        });

        test("toUpper does not modify string with only uppercase letters", [] {
            expect(String("ABCDEF").toUpper(), isEqualTo(String("ABCDEF")));
        });

        test("toUpper modifies only lowercase letters", [] {
            expect(String("13AAbb##").toUpper(), isEqualTo(String("13AABB##")));
        });
    });

    group("stripTrailingWhitespace", [] {
        test("does not strip non-whitespace characters", [] {
            expect(String("Hello World").stripTrailingWhitespace(),
                   isEqualTo(String("Hello World")));
        });

        test("strips only whitespace suffix", [] {
            expect(String("Hello World   ").stripTrailingWhitespace(),
                   isEqualTo(String("Hello World")));
            expect(String("Hello World   \n\n").stripTrailingWhitespace(),
                   isEqualTo(String("Hello World")));
            expect(String("  Hello World \n").stripTrailingWhitespace(),
                   isEqualTo(String("  Hello World")));
        });
    });

    group("strip", [] {
        test("does not strip non-whitespace characters", [] {
            expect(String("Hello World").strip(),
                   isEqualTo(String("Hello World")));
        });

        test("strips whitespace suffix", [] {
            expect(String("Hello World   ").strip(),
                   isEqualTo(String("Hello World")));
            expect(String("Hello World \n\n").strip(),
                   isEqualTo(String("Hello World")));
        });

        test("strips whitespace prefix", [] {
            expect(String("    Hello World").strip(),
                   isEqualTo(String("Hello World")));
            expect(String("\n\n Hello World").strip(),
                   isEqualTo(String("Hello World")));
        });

        test("strips both suffix and prefix", [] {
            expect(String("\n Hell World \n").strip(),
                   isEqualTo(String("Hell World")));
        });
    });

    group("startsWith", [] {
        test("false for empty string and character", [] {
            expect(String("").startsWith('a'), isFalse);
        });

        test("false for empty string and non-empty string", [] {
            expect(String("").startsWith("abc"), isFalse);
        });

        test("true for empty string and empty string", [] {
            expect(String("").startsWith(""), isTrue);
        });

        test("true for string and prefix", [] {
            expect(String("abcdef").startsWith("abc"), isTrue);
        });

        test("false for string and non-prefix", [] {
            expect(String("abcdef").startsWith("abd"), isFalse);
        });

        test("false for string and longer string", [] {
            expect(String("abcdef").startsWith("abcdefg"), isFalse);
        });

        test("true for string and first character", [] {
            expect(String("abcdef").startsWith('a'), isTrue);
        });

        test("false for string and character that is not first character", [] {
            expect(String("abcdef").startsWith('b'), isFalse);
        });
    });

    group("endsWith", [] {
        test("false for empty string and character", [] {
            expect(String("").endsWith('a'), isFalse);
        });

        test("false for empty string and non-empty string", [] {
            expect(String("").endsWith("abc"), isFalse);
        });

        test("true for empty string and empty string", [] {
            expect(String("").endsWith(""), isTrue);
        });

        test("true for string and suffix", [] {
            expect(String("abcdef").endsWith("def"), isTrue);
        });

        test("false for string and non-prefix", [] {
            expect(String("abcdef").endsWith("cef"), isFalse);
        });

        test("false for string and longer string", [] {
            expect(String("abcdef").endsWith("gabcdef"), isFalse);
        });

        test("true for string and last character", [] {
            expect(String("abcdef").endsWith('f'), isTrue);
        });

        test("false for string and character that is not last character", [] {
            expect(String("abcdef").endsWith('e'), isFalse);
        });
    });

    group("containsCharacter", [] {
        test("false for empty string and any character", [] {
            expect(String("").containsCharacter('a'), isFalse);
            expect(String("").containsCharacter('d'), isFalse);
            expect(String("").containsCharacter('#'), isFalse);
        });

        test("false for string and character not in string", [] {
            expect(String("abcdef").containsCharacter('g'), isFalse);
            expect(String("abcdef").containsCharacter('#'), isFalse);
        });

        test("true for string and character in string", [] {
            expect(String("abcdef").containsCharacter('a'), isTrue);
            expect(String("abcdef").containsCharacter('b'), isTrue);
            expect(String("abcdef").containsCharacter('c'), isTrue);
            expect(String("abcdef").containsCharacter('d'), isTrue);
            expect(String("abcdef").containsCharacter('e'), isTrue);
            expect(String("abcdef").containsCharacter('f'), isTrue);
        });
    });

    group("containsSubstring", [] {
        test("false for empty string and any non-empty string", [] {
            expect(String("").containsSubstring("a"), isFalse);
            expect(String("").containsSubstring("something"), isFalse);
            expect(String("").containsSubstring("###"), isFalse);
        });

        test("true for empty string and empty string", [] {
            expect(String("").containsSubstring(""), isTrue);
        });

        test("true for any string and the empty string", [] {
            expect(String("abcdef").containsSubstring(""), isTrue);
            expect(String("something").containsSubstring(""), isTrue);
            expect(String("###").containsSubstring(""), isTrue);
        });

        test("false for string and non-contained substring", [] {
            expect(String("abcdef").containsSubstring("abd"), isFalse);
            expect(String("abcdef").containsSubstring("###"), isFalse);
        });

        test("true for string and substring", [] {
            expect(String("abcdef").containsSubstring(""), isTrue);
            expect(String("abcdef").containsSubstring("a"), isTrue);
            expect(String("abcdef").containsSubstring("b"), isTrue);
            expect(String("abcdef").containsSubstring("c"), isTrue);
            expect(String("abcdef").containsSubstring("d"), isTrue);
            expect(String("abcdef").containsSubstring("e"), isTrue);
            expect(String("abcdef").containsSubstring("f"), isTrue);
            expect(String("abcdef").containsSubstring("ab"), isTrue);
            expect(String("abcdef").containsSubstring("bc"), isTrue);
            expect(String("abcdef").containsSubstring("cd"), isTrue);
            expect(String("abcdef").containsSubstring("de"), isTrue);
            expect(String("abcdef").containsSubstring("ef"), isTrue);
            expect(String("abcdef").containsSubstring("abc"), isTrue);
            expect(String("abcdef").containsSubstring("bcd"), isTrue);
            expect(String("abcdef").containsSubstring("cde"), isTrue);
            expect(String("abcdef").containsSubstring("def"), isTrue);
            expect(String("abcdef").containsSubstring("abcd"), isTrue);
            expect(String("abcdef").containsSubstring("bcde"), isTrue);
            expect(String("abcdef").containsSubstring("cdef"), isTrue);
            expect(String("abcdef").containsSubstring("abcde"), isTrue);
            expect(String("abcdef").containsSubstring("bcdef"), isTrue);
            expect(String("abcdef").containsSubstring("abcdef"), isTrue);
        });
    });

    test("String::copyAsCStr()", [&] {
        String s = "cpp_string";
        char* result = s.copyAsCStr();
        expect(strcmp(result, s.c_str()), isZero);
        free(result);
    });
}
