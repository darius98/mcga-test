#include <iostream>

#include <kktest.hpp>
#include <kktest_ext/matchers/iterable.hpp>
#include <kktest_ext/matchers/numeric.hpp>
#include <kktest_ext/matchers/string.hpp>

#include "kktest/extensions/death/include/kktest_ext/death_impl/matchers.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace kktest::death;
using namespace std;

void matchersTest() {
    auto funcExit0 = [] {
        exit(0);
    };
    auto funcNoExit = [] {
    };
    auto funcOutputExit17 = [] {
        cout << "Hello, World!";
        exit(17);
    };
    auto funcOutputNoExit = [] {
        cout << "Hello, World!";
    };

    group("exits matcher", [&] {
        test("matches exiting function without output", [&] {
            expect(exits.matches(funcExit0));
        });

        test("matches exiting function with output", [&] {
            expect(exits.matches(funcOutputExit17));
        });

        test("does not match non-exiting function without output", [&] {
            expect(!exits.matches(funcNoExit));
        });

        test("does not match non-exiting function with output", [&] {
            expect(!exits.matches(funcOutputNoExit));
        });

        test(".zero matches function with exit code 0", [&] {
            expect(exits.zero.matches(funcExit0));
        });

        test(".zero does not match function with exit code 17", [&] {
            expect(!exits.zero.matches(funcOutputExit17));
        });

        test(".nonZero matches function with exit code 17", [&] {
            expect(exits.nonZero.matches(funcOutputExit17));
        });

        test(".nonZero does not match function with exit code 0", [&] {
            expect(!exits.nonZero.matches(funcExit0));
        });

        test(".withOutput(char[]) matches function with same output", [&] {
            expect(exits.withOutput("Hello, World!").matches(funcOutputExit17));
        });

        test(".withOutput(string) matches function with same output", [&] {
            expect(exits.withOutput(string("Hello, World!"))
                   .matches(funcOutputExit17));
        });

        test(".withOutput(char[]) does not match function with different "
             "output", [&] {
            expect(!exits.withOutput("Hello, World!").matches(funcExit0));
        });

        test(".withOutput(string) does not match function with different "
             "output", [&] {
            expect(!exits.withOutput(string("Hello, World!"))
                   .matches(funcExit0));
        });

        test(".withOutput(matcher) matches function with valid output", [&] {
            expect(exits.withOutput(isSubstringOf("HHHello, World!!!"))
                   .matches(funcOutputExit17));
        });

        test(".withOutput(matcher) does not match function with invalid output",
             [&] {
            expect(!exits.withOutput(hasSize(isZero))
                   .matches(funcOutputExit17));
        });

        test(".withCode(0) matches function that exits with code 0", [&] {
            expect(exits.withCode(0).matches(funcExit0));
        });

        test(".withCode(matcher) matches function that exits with valid code",
             [&] {
            expect(exits.withCode(isZero).matches(funcExit0));
        });

        test(".withCode(0) does not match function that exits with non-zero "
             "exit code", [&] {
            expect(!exits.withCode(0).matches(funcOutputExit17));
        });

        test(".withCode(matcher) does not match function that exits with "
             "invalid exit code", [&] {
            expect(!exits.withCode(isOdd).matches(funcExit0));
        });

        test(".withCode(0) does not match non-exiting function", [&] {
            expect(!exits.withCode(0).matches(funcNoExit));
        });

        test(".withOutput(char[]) does not match non-exiting function", [&] {
            expect(!exits.withOutput("Hello, World!")
                   .matches(funcOutputNoExit));
        });

        test(".withOutput().withCode() matches valid function", [&] {
            expect(exits.withOutput("Hello, World!").withCode(17)
                   .matches(funcOutputExit17));
        });

        test(".withOutput().withCode() does not match function with invalid "
             "exit code", [&] {
            expect(!exits.withOutput("Hello, World!").withCode(19)
                   .matches(funcOutputExit17));
        });

        test(".withOutput().withCode() does not match function with invalid "
             "output", [&] {
            expect(!exits.withOutput("Hello World").withCode(17)
                   .matches(funcOutputExit17));
        });

        test(".withOutput().withCode() does not match non-exiting function",
             [&] {
            expect(!exits.withOutput("Hello, World!").withCode(17)
                   .matches(funcNoExit));
        });

        test(".withOutput & .withCode in different orders and with different"
             " parameter types still compiles", [&] {
            expect(exits.withCode(0).withOutput("").matches(funcExit0));
            expect(exits.withCode(0).withOutput(string("")).matches(funcExit0));
            expect(exits.withCode(isZero).withOutput("").matches(funcExit0));
            expect(exits.withCode(isZero).withOutput(string(""))
                   .matches(funcExit0));
            expect(exits.withCode(0).withOutput(isEmpty).matches(funcExit0));
            expect(exits.withCode(isZero).withOutput(isEmpty)
                   .matches(funcExit0));
            expect(exits.withOutput("").withCode(0).matches(funcExit0));
            expect(exits.withOutput(string("")).withCode(0).matches(funcExit0));
            expect(exits.withOutput("").withCode(isZero).matches(funcExit0));
            expect(exits.withOutput(string("")).withCode(isZero)
                   .matches(funcExit0));
            expect(exits.withOutput(isEmpty).withCode(0).matches(funcExit0));
            expect(exits.withOutput(isEmpty).withCode(isZero)
                   .matches(funcExit0));
        });
    });

    group("hasExited variants", [&] {
        test("hasExited matches status of zero exiting function", [&] {
            auto status = checkDeath(funcExit0);
            expect(hasExited.matches(status));
        });

        test("hasExited matches status of non-zero exiting function", [&] {
            auto status = checkDeath(funcOutputExit17);
            expect(hasExited.matches(status));
        });

        test("hasExited does not match status of non-exiting function", [&] {
            auto status = checkDeath(funcNoExit);
            expect(!hasExited.matches(status));
        });

        test("hasExitedWithCode(0) matches status of zero exiting function",
             [&] {
            auto status = checkDeath(funcExit0);
            expect(hasExitedWithCode(0).matches(status));
        });

        test("hasExitedWithCode(matcher) matches status of valid exiting "
             "function", [&] {
            auto status = checkDeath(funcExit0);
            expect(hasExitedWithCode(isZero).matches(status));
        });

        test("hasExitedWithCode(0) does not match status of non-zero exiting "
             "function", [&] {
            auto status = checkDeath(funcOutputExit17);
            expect(!hasExitedWithCode(0).matches(status));
        });

        test("hasExitedWithCode(matcher) does not match status of invalid "
             "exiting function", [&] {
            auto status = checkDeath(funcOutputExit17);
            expect(!hasExitedWithCode(isZero).matches(status));
        });

        test("hasExitedWithCodeZero matches zero-exiting function", [&] {
            auto status = checkDeath(funcExit0);
            expect(hasExitedWithCodeZero.matches(status));
        });

        test("hasExitedWithNonZeroCode matches non-zero exiting function", [&] {
            auto status = checkDeath(funcOutputExit17);
            expect(hasExitedWithNonZeroCode.matches(status));
        });

        test("hasExitedWithCodeZero does not match non-zero exiting function",
             [&] {
            auto status = checkDeath(funcOutputExit17);
            expect(!hasExitedWithCodeZero.matches(status));
        });

        test("hasExitedWithNonZeroCode does not match zero exiting function",
             [&] {
            auto status = checkDeath(funcExit0);
            expect(!hasExitedWithNonZeroCode.matches(status));
        });
    });

    group("hasOutput", [&] {
        test("(\"\") matches exiting function with empty output", [&] {
            expect(hasOutput("").matches(checkDeath(funcExit0)));
        });

        test("(string(\"\")) matches exiting function with final output", [&] {
            expect(hasOutput(string("")).matches(checkDeath(funcExit0)));
        });

        test("(isEmpty) matches exiting function with empty output", [&] {
            expect(hasOutput(isEmpty).matches(checkDeath(funcExit0)));
        });

        test("(char[]) matches exiting function with same output", [&] {
            expect(hasOutput("Hello, World!")
                   .matches(checkDeath(funcOutputExit17)));
        });

        test("(string) matches exiting function with same output", [&] {
            expect(hasOutput(string("Hello, World!"))
                   .matches(checkDeath(funcOutputExit17)));
        });

        test("(matcher) matches exiting function with valid output", [&] {
            expect(hasOutput(isSubstringOf("HHello, World!!"))
                   .matches(checkDeath(funcOutputExit17)));
        });

        test("(char[]) does not match exiting function with different output",
             [&] {
            expect(!hasOutput("Not Hello World")
                   .matches(checkDeath(funcOutputExit17)));
        });

        test("(matcher) does not match exiting function with invalid output",
             [&] {
            expect(!hasOutput(isEmpty).matches(checkDeath(funcOutputExit17)));
        });

        test("matches non-exiting function if output is valid", [&] {
            expect(hasOutput(isEmpty).matches(checkDeath(funcNoExit)));
        });

        test("does not match non-exiting function if output is invalid", [&] {
            expect(!hasOutput(isNotEmpty).matches(checkDeath(funcNoExit)));
        });
    });
}
