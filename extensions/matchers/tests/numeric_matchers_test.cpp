#include <kktest.hpp>
#include <kktest_ext/matchers_impl/numeric_matchers.hpp>

using namespace kktest;
using namespace kktest::matchers;

void kkTestCase(NumericMatchers) {
    group("isPositive", [&] {
        test("Matches positive numbers", [&] {
            expect(isPositive.matches(3));
            expect(isPositive.matches(10));
            expect(isPositive.matches(1LL << 60));
            expect(isPositive.matches(23u));
            expect(isPositive.matches(35.03));
            expect(isPositive.matches(3.503f));
        });

        test("Does not match zero", [&] {
            expect(!isPositive.matches(0));
            expect(!isPositive.matches(0.0));
            expect(!isPositive.matches(0LL));
        });

        test("Does not match negative numbers", [&] {
            expect(!isPositive.matches(-3));
            expect(!isPositive.matches(-10));
            expect(!isPositive.matches(-(1LL << 60)));
            expect(!isPositive.matches(-35.03));
            expect(!isPositive.matches(-3.503f));
        });
    });

    group("isNegative", [&] {
        test("Does not match positive numbers", [&] {
            expect(!isNegative.matches(3));
            expect(!isNegative.matches(10));
            expect(!isNegative.matches(1LL << 60));
            expect(!isNegative.matches(23u));
            expect(!isNegative.matches(35.03));
            expect(!isNegative.matches(3.503f));
        });

        test("Does not match zero", [&] {
            expect(!isNegative.matches(0));
            expect(!isNegative.matches(0.0));
            expect(!isNegative.matches(0LL));
        });

        test("Matches negative numbers", [&] {
            expect(isNegative.matches(-3));
            expect(isNegative.matches(-10));
            expect(isNegative.matches(-(1LL << 60)));
            expect(isNegative.matches(-35.03));
            expect(isNegative.matches(-3.503f));
        });
    });

    group("isEven", [&] {
        test("Matches even integers", [&] {
            expect(isEven.matches(2));
            expect(isEven.matches(0));
            expect(isEven.matches(-2));
            expect(isEven.matches(1LL << 60));
            expect(isEven.matches(24u));
        });

        test("Does not match odd integers", [&] {
            expect(!isEven.matches(1));
            expect(!isEven.matches(13));
            expect(!isEven.matches(-7));
            expect(!isEven.matches((1LL << 60) - 1));
            expect(!isEven.matches(23u));
        });
    });

    group("isOdd", [&] {
        test("Does not match even integers", [&] {
            expect(!isOdd.matches(2));
            expect(!isOdd.matches(0));
            expect(!isOdd.matches(-2));
            expect(!isOdd.matches(1LL << 60));
            expect(!isOdd.matches(24u));
        });

        test("Matches odd integers", [&] {
            expect(isOdd.matches(1));
            expect(isOdd.matches(13));
            expect(isOdd.matches(-7));
            expect(isOdd.matches((1LL << 60) - 1));
            expect(isOdd.matches(23u));
        });
    });

    group("isZero", [&] {
        test("Does not match positive numbers", [&] {
            expect(!isZero.matches(3));
            expect(!isZero.matches(10));
            expect(!isZero.matches(1LL << 60));
            expect(!isZero.matches(23u));
            expect(!isZero.matches(35.03));
            expect(!isZero.matches(3.503f));
        });

        test("Matches zero", [&] {
            expect(isZero.matches(0));
            expect(isZero.matches(0.0));
            expect(isZero.matches(0LL));
        });

        test("Does not match negative numbers", [&] {
            expect(!isZero.matches(-3));
            expect(!isZero.matches(-10));
            expect(!isZero.matches(-(1LL << 60)));
            expect(!isZero.matches(-35.03));
            expect(!isZero.matches(-3.503f));
        });
    });

    test("isAlmostEqualTo: Changing epsilon changes precision", [&] {
        expect(isAlmostEqualTo(3.0, 0.1).matches(3.05));
        expect(isAlmostEqualTo(3.0, 2).matches(4.99));
        expect(!isAlmostEqualTo(3.0, 1e-12).matches(3 + 1e-9));
    });
}
