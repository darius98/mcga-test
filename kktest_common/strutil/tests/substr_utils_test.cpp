#include <kktest.hpp>
#include <kktest_ext/core_matchers.hpp>

#include <kktest_common/strutil_impl/substr_utils.hpp>

using kktest::expect;
using kktest::group;
using kktest::test;
using kktest::core_matchers::isFalse;
using kktest::core_matchers::isTrue;
using kktest::strutil::startsWith;
using kktest::strutil::endsWith;
using kktest::strutil::containsCharacter;
using kktest::strutil::containsSubstring;
using std::string;

void kkTestCase(StrUtilSubstrUtils) {
    group("startsWith", [] {
        test("false for empty string and character", [] {
            expect(startsWith("", 'a'), isFalse);
        });

        test("false for empty string and non-empty string", [] {
            expect(startsWith("", "abc"), isFalse);
        });

        test("true for empty string and empty string", [] {
            expect(startsWith("", ""), isTrue);
        });

        test("true for string and prefix", [] {
            expect(startsWith("abcdef", "abc"), isTrue);
        });

        test("false for string and non-prefix", [] {
            expect(startsWith("abcdef", "abd"), isFalse);
        });

        test("false for string and longer string", [] {
            expect(startsWith("abcdef", "abcdefg"), isFalse);
        });

        test("true for string and first character", [] {
            expect(startsWith("abcdef", 'a'), isTrue);
        });

        test("false for string and character that is not first character", [] {
            expect(startsWith("abcdef", 'b'), isFalse);
        });
    });

    group("endsWith", [] {
        test("false for empty string and character", [] {
            expect(endsWith("", 'a'), isFalse);
        });

        test("false for empty string and non-empty string", [] {
            expect(endsWith("", "abc"), isFalse);
        });

        test("true for empty string and empty string", [] {
            expect(endsWith("", ""), isTrue);
        });

        test("true for string and suffix", [] {
            expect(endsWith("abcdef", "def"), isTrue);
        });

        test("false for string and non-prefix", [] {
            expect(endsWith("abcdef", "cef"), isFalse);
        });

        test("false for string and longer string", [] {
            expect(endsWith("abcdef", "gabcdef"), isFalse);
        });

        test("true for string and last character", [] {
            expect(endsWith("abcdef", 'f'), isTrue);
        });

        test("false for string and character that is not last character", [] {
            expect(endsWith("abcdef", 'e'), isFalse);
        });
    });

    group("containsCharacter", [] {
        test("false for empty string and any character", [] {
            expect(containsCharacter("", 'a'), isFalse);
            expect(containsCharacter("", 'd'), isFalse);
            expect(containsCharacter("", '#'), isFalse);
        });

        test("false for string and character not in string", [] {
            expect(containsCharacter("abcdef", 'g'), isFalse);
            expect(containsCharacter("abcdef", '#'), isFalse);
        });

        test("true for string and character in string", [] {
            expect(containsCharacter("abcdef", 'a'), isTrue);
            expect(containsCharacter("abcdef", 'b'), isTrue);
            expect(containsCharacter("abcdef", 'c'), isTrue);
            expect(containsCharacter("abcdef", 'd'), isTrue);
            expect(containsCharacter("abcdef", 'e'), isTrue);
            expect(containsCharacter("abcdef", 'f'), isTrue);
        });
    });

    group("containsSubstring", [] {
        test("false for empty string and any non-empty string", [] {
            expect(containsSubstring("", "a"), isFalse);
            expect(containsSubstring("", "something"), isFalse);
            expect(containsSubstring("", "###"), isFalse);
        });

        test("true for empty string and empty string", [] {
            expect(containsSubstring("", ""), isTrue);
        });

        test("true for any string and the empty string", [] {
            expect(containsSubstring("abcdef", ""), isTrue);
            expect(containsSubstring("something", ""), isTrue);
            expect(containsSubstring("###", ""), isTrue);
        });

        test("false for string and non-contained substring", [] {
            expect(containsSubstring("abcdef", "abd"), isFalse);
            expect(containsSubstring("abcdef", "###"), isFalse);
        });

        test("true for string and substring", [] {
            expect(containsSubstring("abcdef", ""), isTrue);
            expect(containsSubstring("abcdef", "a"), isTrue);
            expect(containsSubstring("abcdef", "b"), isTrue);
            expect(containsSubstring("abcdef", "c"), isTrue);
            expect(containsSubstring("abcdef", "d"), isTrue);
            expect(containsSubstring("abcdef", "e"), isTrue);
            expect(containsSubstring("abcdef", "f"), isTrue);
            expect(containsSubstring("abcdef", "ab"), isTrue);
            expect(containsSubstring("abcdef", "bc"), isTrue);
            expect(containsSubstring("abcdef", "cd"), isTrue);
            expect(containsSubstring("abcdef", "de"), isTrue);
            expect(containsSubstring("abcdef", "ef"), isTrue);
            expect(containsSubstring("abcdef", "abc"), isTrue);
            expect(containsSubstring("abcdef", "bcd"), isTrue);
            expect(containsSubstring("abcdef", "cde"), isTrue);
            expect(containsSubstring("abcdef", "def"), isTrue);
            expect(containsSubstring("abcdef", "abcd"), isTrue);
            expect(containsSubstring("abcdef", "bcde"), isTrue);
            expect(containsSubstring("abcdef", "cdef"), isTrue);
            expect(containsSubstring("abcdef", "abcde"), isTrue);
            expect(containsSubstring("abcdef", "bcdef"), isTrue);
            expect(containsSubstring("abcdef", "abcdef"), isTrue);
        });
    });
}
