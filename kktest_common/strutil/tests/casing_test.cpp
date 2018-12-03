#include <kktest.hpp>
#include <kktest_ext/core_matchers.hpp>

#include <kktest_common/strutil_impl/casing.hpp>

using kktest::expect;
using kktest::group;
using kktest::tearDown;
using kktest::test;
using kktest::core_matchers::isEqualTo;
using kktest::strutil::toLower;
using kktest::strutil::toUpper;
using kktest::strutil::stripTrailingWhitespace;
using kktest::strutil::strip;
using std::string;

void kkTestCase(StrUtilCasing) {
    group("toLower", [] {
        test("toLower does not modify string with no letters", [] {
            expect(toLower("12345!@#$"), isEqualTo(string("12345!@#$")));
        });

        test("toLower does not modify string with only lowercase letters", [] {
            expect(toLower("abcdef"), isEqualTo(string("abcdef")));
        });

        test("toLower modifies only uppercase letters", [] {
            expect(toLower("123AAAbbb###"), isEqualTo(string("123aaabbb###")));
        });
    });

    group("toUpper", [] {
        test("toUpper does not modify string with no letters", [] {
            expect(toUpper("12345!@#$"), isEqualTo(string("12345!@#$")));
        });

        test("toUpper does not modify string with only uppercase letters", [] {
            expect(toUpper("ABCDEF"), isEqualTo(string("ABCDEF")));
        });

        test("toUpper modifies only lowercase letters", [] {
            expect(toUpper("123AAAbbb###"), isEqualTo(string("123AAABBB###")));
        });
    });

    group("stripTrailingWhitespace", [] {
        test("does not strip non-whitespace characters", [] {
            expect(stripTrailingWhitespace("Hello World"), isEqualTo(string("Hello World")));
        });

        test("strips only whitespace suffix", [] {
            expect(stripTrailingWhitespace("Hello World   "), isEqualTo(string("Hello World")));
            expect(stripTrailingWhitespace("Hello World   \n\n"), isEqualTo(string("Hello World")));
            expect(stripTrailingWhitespace("  Hello World \n"), isEqualTo(string("  Hello World")));
        });
    });

    group("strip", [] {
        test("does not strip non-whitespace characters", [] {
            expect(strip("Hello World"), isEqualTo(string("Hello World")));
        });

        test("strips whitespace suffix", [] {
            expect(strip("Hello World   "), isEqualTo(string("Hello World")));
            expect(strip("Hello World \n\n"), isEqualTo(string("Hello World")));
        });

        test("strips whitespace prefix", [] {
            expect(strip("    Hello World"), isEqualTo(string("Hello World")));
            expect(strip("\n\n Hello World"), isEqualTo(string("Hello World")));
        });

        test("strips both suffix and prefix", [] {
            expect(strip("\n Hell World \n"), isEqualTo(string("Hell World")));
        });
    });
}
