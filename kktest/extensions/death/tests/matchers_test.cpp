#include <iostream>

#include <kktest.hpp>
#include <kktest_ext/matchers/iterable.hpp>
#include <kktest_ext/matchers/numeric.hpp>
#include <kktest_ext/matchers/string.hpp>
#include <kktest_ext/matchers/testing_helpers.hpp>

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
            EXPECT_MATCHER_MATCHES(funcExit0, exits);
        });

        test("matches exiting function with output", [&] {
            EXPECT_MATCHER_MATCHES(funcOutputExit17, exits);
        });

        test("does not match non-exiting function without output", [&] {
            EXPECT_MATCHER_FAILS(funcNoExit, exits);
        });

        test("does not match non-exiting function with output", [&] {
            EXPECT_MATCHER_FAILS(funcOutputNoExit, exits);
        });

        test(".zero matches function with exit code 0", [&] {
            EXPECT_MATCHER_MATCHES(funcExit0, exits.zero);
        });

        test(".zero does not match function with exit code 17", [&] {
            EXPECT_MATCHER_FAILS(funcOutputExit17, exits.zero);
        });

        test(".nonZero matches function with exit code 17", [&] {
            EXPECT_MATCHER_MATCHES(funcOutputExit17, exits.nonZero);
        });

        test(".nonZero does not match function with exit code 0", [&] {
            EXPECT_MATCHER_FAILS(funcExit0, exits.nonZero);
        });

        test(".withOutput(char[]) matches function with same output", [&] {
            EXPECT_MATCHER_MATCHES(funcOutputExit17,
                                   exits.withOutput("Hello, World!"));
        });

        test(".withOutput(string) matches function with same output", [&] {
            EXPECT_MATCHER_MATCHES(funcOutputExit17,
                                   exits.withOutput(string("Hello, World!")));
        });

        test(".withOutput(char[]) does not match function with different "
             "output", [&] {
            EXPECT_MATCHER_FAILS(funcExit0, exits.withOutput("Hello, World!"));
        });

        test(".withOutput(string) does not match function with different "
             "output", [&] {
            EXPECT_MATCHER_FAILS(funcExit0,
                                 exits.withOutput(string("Hello, World!")));
        });

        test(".withOutput(matcher) matches function with valid output", [&] {
            EXPECT_MATCHER_MATCHES(funcOutputExit17, exits.withOutput(
                    isSubstringOf("HHHello, World!!!")));
        });

        test(".withOutput(matcher) does not match function with invalid output",
             [&] {
            EXPECT_MATCHER_FAILS(funcOutputExit17,
                                 exits.withOutput(hasSize(isZero)));
        });

        test(".withCode(0) matches function that exits with code 0", [&] {
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(0));
        });

        test(".withCode(matcher) matches function that exits with valid code",
             [&] {
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(isZero));
        });

        test(".withCode(0) does not match function that exits with non-zero "
             "exit code", [&] {
            EXPECT_MATCHER_FAILS(funcOutputExit17, exits.withCode(0));
        });

        test(".withCode(matcher) does not match function that exits with "
             "invalid exit code", [&] {
            EXPECT_MATCHER_FAILS(funcExit0, exits.withCode(isOdd));
        });

        test(".withCode(0) does not match non-exiting function", [&] {
            EXPECT_MATCHER_FAILS(funcNoExit, exits.withCode(0));
        });

        test(".withOutput(char[]) does not match non-exiting function", [&] {
            EXPECT_MATCHER_FAILS(funcOutputNoExit,
                                 exits.withOutput("Hello, World!"));
        });

        test(".withOutput().withCode() matches valid function", [&] {
            EXPECT_MATCHER_MATCHES(funcOutputExit17,
                                   exits.withOutput("Hello, World!")
                                        .withCode(17));
        });

        test(".withOutput().withCode() does not match function with invalid "
             "exit code", [&] {
            EXPECT_MATCHER_FAILS(funcOutputExit17,
                                 exits.withOutput("Hello, World!")
                                      .withCode(19));
        });

        test(".withOutput().withCode() does not match function with invalid "
             "output", [&] {
            EXPECT_MATCHER_FAILS(funcOutputExit17,
                                 exits.withOutput("Hello World").withCode(17));
        });

        test(".withOutput().withCode() does not match non-exiting function",
             [&] {
            EXPECT_MATCHER_FAILS(funcNoExit,
                                 exits.withOutput("Hello, World!")
                                      .withCode(17));
        });

        test(".withOutput & .withCode in different orders and with different"
             " parameter types still compiles", [&] {
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(0).withOutput(""));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(0)
                                                   .withOutput(string("")));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(isZero)
                                                   .withOutput(""));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(isZero)
                                                   .withOutput(string("")));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(0)
                                                   .withOutput(isEmpty));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(isZero)
                                                   .withOutput(isEmpty));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withOutput("").withCode(0));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withOutput(string(""))
                                                   .withCode(0));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withOutput("")
                                                   .withCode(isZero));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withOutput(string(""))
                                                   .withCode(isZero));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withOutput(isEmpty)
                                                   .withCode(0));
            EXPECT_MATCHER_MATCHES(funcExit0, exits.withOutput(isEmpty)
                                                   .withCode(isZero));
        });
    });

    group("hasExited variants", [&] {
        test("hasExited matches status of zero exiting function", [&] {
            auto status = checkDeath(funcExit0);
            EXPECT_MATCHER_MATCHES(status, hasExited);
        });

        test("hasExited matches status of non-zero exiting function", [&] {
            auto status = checkDeath(funcOutputExit17);
            EXPECT_MATCHER_MATCHES(status, hasExited);
        });

        test("hasExited does not match status of non-exiting function", [&] {
            auto status = checkDeath(funcNoExit);
            EXPECT_MATCHER_FAILS(status, hasExited);
        });

        test("hasExitedWithCode(0) matches status of zero exiting function",
             [&] {
            auto status = checkDeath(funcExit0);
            EXPECT_MATCHER_MATCHES(status, hasExitedWithCode(0));
        });

        test("hasExitedWithCode(matcher) matches status of valid exiting "
             "function", [&] {
            auto status = checkDeath(funcExit0);
            EXPECT_MATCHER_MATCHES(status, hasExitedWithCode(isZero));
        });

        test("hasExitedWithCode(0) does not match status of non-zero exiting "
             "function", [&] {
            auto status = checkDeath(funcOutputExit17);
            EXPECT_MATCHER_FAILS(status, hasExitedWithCode(0));
        });

        test("hasExitedWithCode(matcher) does not match status of invalid "
             "exiting function", [&] {
            auto status = checkDeath(funcOutputExit17);
            EXPECT_MATCHER_FAILS(status, hasExitedWithCode(isZero));
        });

        test("hasExitedWithCodeZero matches zero-exiting function", [&] {
            auto status = checkDeath(funcExit0);
            EXPECT_MATCHER_MATCHES(status, hasExitedWithCodeZero);
        });

        test("hasExitedWithNonZeroCode matches non-zero exiting function", [&] {
            auto status = checkDeath(funcOutputExit17);
            EXPECT_MATCHER_MATCHES(status, hasExitedWithNonZeroCode);
        });

        test("hasExitedWithCodeZero does not match non-zero exiting function",
             [&] {
            auto status = checkDeath(funcOutputExit17);
            EXPECT_MATCHER_FAILS(status, hasExitedWithCodeZero);
        });

        test("hasExitedWithNonZeroCode does not match zero exiting function",
             [&] {
            auto status = checkDeath(funcExit0);
            EXPECT_MATCHER_FAILS(status, hasExitedWithNonZeroCode);
        });
    });

    group("hasExitedWithOutput", [&] {
        test("(\"\") matches exiting function with empty output", [&] {
            EXPECT_MATCHER_MATCHES(checkDeath(funcExit0),
                                   hasExitedWithOutput(""));
        });

        test("(string(\"\")) matches exiting function with final output", [&] {
            EXPECT_MATCHER_MATCHES(checkDeath(funcExit0),
                                   hasExitedWithOutput(string("")));
        });

        test("(isEmpty) matches exiting function with empty output", [&] {
            EXPECT_MATCHER_MATCHES(checkDeath(funcExit0),
                                   hasExitedWithOutput(isEmpty));
        });

        test("(char[]) matches exiting function with same output", [&] {
            EXPECT_MATCHER_MATCHES(checkDeath(funcOutputExit17),
                                   hasExitedWithOutput("Hello, World!"));
        });

        test("(string) matches exiting function with same output", [&] {
            EXPECT_MATCHER_MATCHES(
                    checkDeath(funcOutputExit17),
                    hasExitedWithOutput(string("Hello, World!")));
        });

        test("(matcher) matches exiting function with valid output", [&] {
            EXPECT_MATCHER_MATCHES(
                    checkDeath(funcOutputExit17),
                    hasExitedWithOutput(isSubstringOf("HHello, World!!")));
        });

        test("(char[]) does not match exiting function with different output",
             [&] {
            EXPECT_MATCHER_FAILS(checkDeath(funcOutputExit17),
                                 hasExitedWithOutput("Not Hello World"));
        });

        test("(matcher) does not match exiting function with invalid output",
             [&] {
            EXPECT_MATCHER_FAILS(checkDeath(funcOutputExit17),
                                 hasExitedWithOutput(isEmpty));
        });

        test("matches non-exiting function if output is valid", [&] {
            EXPECT_MATCHER_MATCHES(checkDeath(funcNoExit),
                                   hasExitedWithOutput(isEmpty));
        });

        test("does not match non-exiting function if output is invalid", [&] {
            EXPECT_MATCHER_FAILS(checkDeath(funcNoExit),
                                 hasExitedWithOutput(isNotEmpty));
        });
    });
}
