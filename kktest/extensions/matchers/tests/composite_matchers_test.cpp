#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers/composite.hpp"
#include "kktest/extensions/matchers/include/kktest_ext/matchers/testing_helpers.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace std;

void compositeMatchersTest() {
    group("both", [] {
        test("both(fitting value, fitting value)", [&] {
            EXPECT_MATCHER_MATCHES(3, both(3, 3));
        });

        test("both(fitting value, not-fitting value)", [&] {
            EXPECT_MATCHER_FAILS(3, both(3, 5));
        });

        test("both(not-fitting value, fitting value)", [&] {
            EXPECT_MATCHER_FAILS(5, both(3, 5));
        });

        test("both(not-fitting value, not-fitting value)", [&] {
            EXPECT_MATCHER_FAILS(4, both(3, 5));
        });

        test("both(fitting value, fitting matcher)", [] {
            EXPECT_MATCHER_MATCHES(3, both(3, isLessThan(4)));
        });

        test("both(fitting value, un-fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(3, both(3, isLessThan(2)));
        });

        test("both(un-fitting value, fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(5, both(3, isLessThan(6)));
        });

        test("both(un-fitting value, un-fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(10, both(3, isLessThan(6)));
        });

        test("both(fitting matcher, fitting value)", [] {
            EXPECT_MATCHER_MATCHES(3, both(isLessThan(4), 3));
        });

        test("both(un-fitting matcher, fitting value)", [] {
            EXPECT_MATCHER_FAILS(3, both(isLessThan(2), 3));
        });

        test("both(fitting matcher, un-fitting value)", [] {
            EXPECT_MATCHER_FAILS(3, both(isLessThan(6), 2));
        });

        test("both(un-fitting matcher, un-fitting value)", [] {
            EXPECT_MATCHER_FAILS(10, both(isLessThan(6), 2));
        });

        test("both(fitting matcher, fitting matcher)", [] {
            EXPECT_MATCHER_MATCHES(3, both(isLessThan(7), isGreaterThan(2)));
        });

        test("both(fitting matcher, un-fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(1, both(isLessThan(7), isGreaterThan(2)));
        });

        test("both(un-fitting matcher, fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(1, both(isLessThan(-2), isGreaterThan(0)));
        });

        test("both(un-fitting matcher, un-fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(1, both(isLessThan(-2), isGreaterThan(2)));
        });
    });

    group("either", [] {
        test("either(fitting value, fitting value)", [&] {
            EXPECT_MATCHER_MATCHES(3, either(3, 3));
        });

        test("either(fitting value, not-fitting value)", [&] {
            EXPECT_MATCHER_MATCHES(3, either(3, 5));
        });

        test("either(not-fitting value, fitting value)", [&] {
            EXPECT_MATCHER_MATCHES(5, either(3, 5));
        });

        test("either(not-fitting value, not-fitting value)", [&] {
            EXPECT_MATCHER_FAILS(4, either(3, 5));
        });

        test("either(fitting value, fitting matcher)", [] {
            EXPECT_MATCHER_MATCHES(3, either(3, isLessThan(4)));
        });

        test("either(fitting value, un-fitting matcher)", [] {
            EXPECT_MATCHER_MATCHES(3, either(3, isLessThan(2)));
        });

        test("either(un-fitting value, fitting matcher)", [] {
            EXPECT_MATCHER_MATCHES(5, either(3, isLessThan(6)));
        });

        test("either(un-fitting value, un-fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(10, either(3, isLessThan(6)));
        });

        test("either(fitting matcher, fitting value)", [] {
            EXPECT_MATCHER_MATCHES(3, either(isLessThan(4), 3));
        });

        test("either(un-fitting matcher, fitting value)", [] {
            EXPECT_MATCHER_MATCHES(3, either(isLessThan(2), 3));
        });

        test("either(fitting matcher, un-fitting value)", [] {
            EXPECT_MATCHER_MATCHES(5, either(isLessThan(6), 3));
        });

        test("either(un-fitting matcher, un-fitting value)", [] {
            EXPECT_MATCHER_FAILS(10, either(isLessThan(6), 3));
        });

        test("either(fitting matcher, fitting matcher)", [] {
            EXPECT_MATCHER_MATCHES(3, either(isLessThan(7), isGreaterThan(2)));
        });

        test("either(fitting matcher, un-fitting matcher)", [] {
            EXPECT_MATCHER_MATCHES(1, either(isLessThan(7), isGreaterThan(2)));
        });

        test("either(fitting matcher, un-fitting matcher)", [] {
            EXPECT_MATCHER_MATCHES(10, either(isLessThan(7), isGreaterThan(2)));
        });

        test("either(un-fitting matcher, un-fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(10, either(isLessThan(7), isGreaterThan(20)));
        });
    });

    group("isNot", [] {
        test("isNot(fitting value)", [] {
            EXPECT_MATCHER_FAILS(10, isNot(10));
        });

        test("isNot(un-fitting value)", [] {
            EXPECT_MATCHER_MATCHES(10, isNot(5));
        });

        test("isNot(fitting matcher)", [] {
            EXPECT_MATCHER_FAILS(15, isNot(isGreaterThan(10)));
        });

        test("isNot(un-fitting matcher", [] {
            EXPECT_MATCHER_MATCHES(7, isNot(isGreaterThan(10)));
        });
    });
}
