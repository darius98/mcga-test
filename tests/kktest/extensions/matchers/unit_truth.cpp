#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers/testing_helpers.hpp"
#include "kktest/extensions/matchers/include/kktest_ext/matchers/truth.hpp"

using namespace kktest;
using namespace kktest::matchers;

TEST_CASE(truth, "Matchers extension: truth") {
    test("isTrue matcher matches true",
         [] { EXPECT_MATCHER_MATCHES(true, isTrue); });

    test("isTrue matcher does not match false",
         [] { EXPECT_MATCHER_FAILS(false, isTrue); });

    test("isFalse matcher does not match true",
         [] { EXPECT_MATCHER_FAILS(true, isFalse); });

    test("isFalse matcher matches false",
         [] { EXPECT_MATCHER_MATCHES(false, isFalse); });
}
