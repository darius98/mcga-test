#include <kktest>
#include <kktest_ext/core_matchers>

using namespace kktest;

void kkTestCase(NumericMatchers) {
    kkGroup("isPositive", [&]() {
        kkTest("Matches positive numbers", [&]() {
            kkExpect(isPositive.matches(3));
            kkExpect(isPositive.matches(10));
            kkExpect(isPositive.matches(1LL << 60));
            kkExpect(isPositive.matches(23u));
            kkExpect(isPositive.matches(35.03));
            kkExpect(isPositive.matches(3.503f));
        });

        kkTest("Does not match zero", [&]() {
            kkExpect(!isPositive.matches(0));
            kkExpect(!isPositive.matches(0.0));
            kkExpect(!isPositive.matches(0LL));
        });

        kkTest("Does not match negative numbers", [&]() {
            kkExpect(!isPositive.matches(-3));
            kkExpect(!isPositive.matches(-10));
            kkExpect(!isPositive.matches(-(1LL << 60)));
            kkExpect(!isPositive.matches(-35.03));
            kkExpect(!isPositive.matches(-3.503f));
        });
    });

    kkGroup("isNegative", [&]() {
        kkTest("Does not match positive numbers", [&]() {
            kkExpect(!isNegative.matches(3));
            kkExpect(!isNegative.matches(10));
            kkExpect(!isNegative.matches(1LL << 60));
            kkExpect(!isNegative.matches(23u));
            kkExpect(!isNegative.matches(35.03));
            kkExpect(!isNegative.matches(3.503f));
        });

        kkTest("Does not match zero", [&]() {
            kkExpect(!isNegative.matches(0));
            kkExpect(!isNegative.matches(0.0));
            kkExpect(!isNegative.matches(0LL));
        });

        kkTest("Matches negative numbers", [&]() {
            kkExpect(isNegative.matches(-3));
            kkExpect(isNegative.matches(-10));
            kkExpect(isNegative.matches(-(1LL << 60)));
            kkExpect(isNegative.matches(-35.03));
            kkExpect(isNegative.matches(-3.503f));
        });
    });

    kkGroup("isEven", [&]() {
        kkTest("Matches even integers", [&]() {
            kkExpect(isEven.matches(2));
            kkExpect(isEven.matches(0));
            kkExpect(isEven.matches(-2));
            kkExpect(isEven.matches(1LL << 60));
            kkExpect(isEven.matches(24u));
        });

        kkTest("Does not match odd integers", [&]() {
            kkExpect(!isEven.matches(1));
            kkExpect(!isEven.matches(13));
            kkExpect(!isEven.matches(-7));
            kkExpect(!isEven.matches((1LL << 60) - 1));
            kkExpect(!isEven.matches(23u));
        });
    });

    kkGroup("isOdd", [&]() {
        kkTest("Does not match even integers", [&]() {
            kkExpect(!isOdd.matches(2));
            kkExpect(!isOdd.matches(0));
            kkExpect(!isOdd.matches(-2));
            kkExpect(!isOdd.matches(1LL << 60));
            kkExpect(!isOdd.matches(24u));
        });

        kkTest("Matches odd integers", [&]() {
            kkExpect(isOdd.matches(1));
            kkExpect(isOdd.matches(13));
            kkExpect(isOdd.matches(-7));
            kkExpect(isOdd.matches((1LL << 60) - 1));
            kkExpect(isOdd.matches(23u));
        });
    });

    kkGroup("isZero", [&]() {
        kkTest("Does not match positive numbers", [&]() {
            kkExpect(!isZero.matches(3));
            kkExpect(!isZero.matches(10));
            kkExpect(!isZero.matches(1LL << 60));
            kkExpect(!isZero.matches(23u));
            kkExpect(!isZero.matches(35.03));
            kkExpect(!isZero.matches(3.503f));
        });

        kkTest("Matches zero", [&]() {
            kkExpect(isZero.matches(0));
            kkExpect(isZero.matches(0.0));
            kkExpect(isZero.matches(0LL));
        });

        kkTest("Does not match negative numbers", [&]() {
            kkExpect(!isZero.matches(-3));
            kkExpect(!isZero.matches(-10));
            kkExpect(!isZero.matches(-(1LL << 60)));
            kkExpect(!isZero.matches(-35.03));
            kkExpect(!isZero.matches(-3.503f));
        });
    });

    kkGroup("isAlmostEqualTo", [&]() {
        kkTest("Default epsilon is high precision", [&]() {
            kkExpect(isAlmostEqualTo(3.0).matches(3 + 1e-9));
            kkExpect(isAlmostEqualTo(3.0).matches(3 + 1e-12));
            kkExpect(!isAlmostEqualTo(3.00).matches(3.01));
            kkExpect(!isAlmostEqualTo(3.0).matches(3.1));
        });

        kkTest("Changing epsilon changes precision", [&]() {
            kkExpect(isAlmostEqualTo(3.0, 0.1).matches(3.05));
            kkExpect(isAlmostEqualTo(3.0, 2).matches(4.99));
            kkExpect(!isAlmostEqualTo(3.0, 1e-12).matches(3 + 1e-9));
        });
    });
}
