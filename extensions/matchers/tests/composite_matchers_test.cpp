#include <kktest.hpp>

#include "extensions/matchers/include/kktest_ext/matchers_impl/composite_matchers.hpp"

using namespace kktest;
using namespace kktest::matchers;

void kkTestCase(CompositeMatchers) {
    group("both", [] {
        test("both(fitting value, fitting value)", [&] {
            expect(both(3, 3).matches(3));
        });

        test("both(fitting value, not-fitting value)", [&] {
            expect(!both(3, 5).matches(3));
        });

        test("both(not-fitting value, fitting value)", [&] {
            expect(!both(3, 5).matches(5));
        });

        test("both(not-fitting value, not-fitting value)", [&] {
            expect(!both(3, 5).matches(4));
        });

        test("both(fitting value, fitting matcher)", [] {
            expect(both(3, isLessThan(4)).matches(3));
        });

        test("both(fitting value, un-fitting matcher)", [] {
            expect(!both(3, isLessThan(2)).matches(3));
        });

        test("both(un-fitting value, fitting matcher)", [] {
            expect(!both(3, isLessThan(6)).matches(5));
        });

        test("both(un-fitting value, un-fitting matcher)", [] {
            expect(!both(3, isLessThan(6)).matches(10));
        });

        test("both(fitting matcher, fitting value)", [] {
            expect(both(isLessThan(4), 3).matches(3));
        });

        test("both(un-fitting matcher, fitting value)", [] {
            expect(!both(isLessThan(2), 3).matches(3));
        });

        test("both(fitting matcher, un-fitting value)", [] {
            expect(!both(isLessThan(6), 3).matches(5));
        });

        test("both(un-fitting matcher, un-fitting value)", [] {
            expect(!both(isLessThan(6), 3).matches(10));
        });

        test("both(fitting matcher, fitting matcher)", [] {
            expect(both(isLessThan(7), isGreaterThan(2)).matches(3));
        });

        test("both(fitting matcher, un-fitting matcher)", [] {
            expect(!both(isLessThan(7), isGreaterThan(2)).matches(1));
        });

        test("both(fitting matcher, un-fitting matcher)", [] {
            expect(!both(isLessThan(7), isGreaterThan(2)).matches(10));
        });

        test("both(un-fitting matcher, un-fitting matcher)", [] {
            expect(!both(isLessThan(7), isGreaterThan(20)).matches(10));
        });
    });

    group("either", [] {
        test("either(fitting value, fitting value)", [&] {
            expect(either(3, 3).matches(3));
        });

        test("either(fitting value, not-fitting value)", [&] {
            expect(either(3, 5).matches(3));
        });

        test("either(not-fitting value, fitting value)", [&] {
            expect(either(3, 5).matches(5));
        });

        test("either(not-fitting value, not-fitting value)", [&] {
            expect(!either(3, 5).matches(4));
        });

        test("either(fitting value, fitting matcher)", [] {
            expect(either(3, isLessThan(4)).matches(3));
        });

        test("either(fitting value, un-fitting matcher)", [] {
            expect(either(3, isLessThan(2)).matches(3));
        });

        test("either(un-fitting value, fitting matcher)", [] {
            expect(either(3, isLessThan(6)).matches(5));
        });

        test("either(un-fitting value, un-fitting matcher)", [] {
            expect(!either(3, isLessThan(6)).matches(10));
        });

        test("either(fitting matcher, fitting value)", [] {
            expect(either(isLessThan(4), 3).matches(3));
        });

        test("either(un-fitting matcher, fitting value)", [] {
            expect(either(isLessThan(2), 3).matches(3));
        });

        test("either(fitting matcher, un-fitting value)", [] {
            expect(either(isLessThan(6), 3).matches(5));
        });

        test("either(un-fitting matcher, un-fitting value)", [] {
            expect(!either(isLessThan(6), 3).matches(10));
        });

        test("either(fitting matcher, fitting matcher)", [] {
            expect(either(isLessThan(7), isGreaterThan(2)).matches(3));
        });

        test("either(fitting matcher, un-fitting matcher)", [] {
            expect(either(isLessThan(7), isGreaterThan(2)).matches(1));
        });

        test("either(fitting matcher, un-fitting matcher)", [] {
            expect(either(isLessThan(7), isGreaterThan(2)).matches(10));
        });

        test("either(un-fitting matcher, un-fitting matcher)", [] {
            expect(!either(isLessThan(7), isGreaterThan(20)).matches(10));
        });
    });

    group("isNot", [] {
        test("isNot(fitting value)", [] {
            expect(!isNot(10).matches(10));
        });

        test("isNot(un-fitting value)", [] {
            expect(isNot(5).matches(10));
        });

        test("isNot(fitting matcher)", [] {
            expect(!isNot(isGreaterThan(10)).matches(15));
        });

        test("isNot(un-fitting matcher", [] {
            expect(isNot(isGreaterThan(10)).matches(7));
        });
    });
}
