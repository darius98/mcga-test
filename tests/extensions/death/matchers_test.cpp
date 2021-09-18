#include <iostream>

#include "mcga/matchers/testing_helpers.hpp"
#include "mcga/test.hpp"
#include "mcga/test_ext/matchers.hpp"

#include "mcga/test_ext/death/matchers.hpp"

using namespace mcga::test;
using namespace mcga::matchers;
using namespace mcga::test::death;

TEST_CASE(matchers, "Death extension: matchers") {
    auto funcExit0 = [] { exit(0); };
    auto funcExit17 = [] { exit(17); };
    auto funcNoExit = [] {};

    group("exits matcher", [&] {
        test("matches exiting function with exit code 0",
             [&] { EXPECT_MATCHER_MATCHES(funcExit0, exits); });

        test("matches exiting function with exit code 17",
             [&] { EXPECT_MATCHER_MATCHES(funcExit17, exits); });

        test("does not match non-exiting function",
             [&] { EXPECT_MATCHER_FAILS(funcNoExit, exits); });

        test(".zero matches function with exit code 0",
             [&] { EXPECT_MATCHER_MATCHES(funcExit0, exits.zero); });

        test(".zero does not match function with exit code 17",
             [&] { EXPECT_MATCHER_FAILS(funcExit17, exits.zero); });

        test(".nonZero matches function with exit code 17",
             [&] { EXPECT_MATCHER_MATCHES(funcExit17, exits.nonZero); });

        test(".nonZero does not match function with exit code 0",
             [&] { EXPECT_MATCHER_FAILS(funcExit0, exits.nonZero); });

        test(".withCode(0) matches function that exits with code 0",
             [&] { EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(0)); });

        test(
          ".withCode(matcher) matches function that exits with valid code",
          [&] { EXPECT_MATCHER_MATCHES(funcExit0, exits.withCode(isZero)); });

        test(".withCode(0) does not match function that exits with non-zero "
             "exit code",
             [&] { EXPECT_MATCHER_FAILS(funcExit17, exits.withCode(0)); });

        test(".withCode(matcher) does not match function that exits with "
             "invalid exit code",
             [&] { EXPECT_MATCHER_FAILS(funcExit0, exits.withCode(isOdd)); });

        test(".withCode(0) does not match non-exiting function",
             [&] { EXPECT_MATCHER_FAILS(funcNoExit, exits.withCode(0)); });
    });

    group("hasExited variants", [&] {
        test("hasExited matches status of zero exiting function", [&] {
            auto status = checkDeath(funcExit0);
            EXPECT_MATCHER_MATCHES(status, hasExited);
        });

        test("hasExited matches status of non-zero exiting function", [&] {
            auto status = checkDeath(funcExit17);
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
             "function",
             [&] {
                 auto status = checkDeath(funcExit0);
                 EXPECT_MATCHER_MATCHES(status, hasExitedWithCode(isZero));
             });

        test("hasExitedWithCode(0) does not match status of non-zero exiting "
             "function",
             [&] {
                 auto status = checkDeath(funcExit17);
                 EXPECT_MATCHER_FAILS(status, hasExitedWithCode(0));
             });

        test("hasExitedWithCode(matcher) does not match status of invalid "
             "exiting function",
             [&] {
                 auto status = checkDeath(funcExit17);
                 EXPECT_MATCHER_FAILS(status, hasExitedWithCode(isZero));
             });

        test("hasExitedWithCodeZero matches zero-exiting function", [&] {
            auto status = checkDeath(funcExit0);
            EXPECT_MATCHER_MATCHES(status, hasExitedWithCodeZero);
        });

        test("hasExitedWithNonZeroCode matches non-zero exiting function", [&] {
            auto status = checkDeath(funcExit17);
            EXPECT_MATCHER_MATCHES(status, hasExitedWithNonZeroCode);
        });

        test("hasExitedWithCodeZero does not match non-zero exiting function",
             [&] {
                 auto status = checkDeath(funcExit17);
                 EXPECT_MATCHER_FAILS(status, hasExitedWithCodeZero);
             });

        test("hasExitedWithNonZeroCode does not match zero exiting function",
             [&] {
                 auto status = checkDeath(funcExit0);
                 EXPECT_MATCHER_FAILS(status, hasExitedWithNonZeroCode);
             });
    });
}
