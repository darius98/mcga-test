#include <kktest>
#include <kktest_matchers>

using namespace kktest;
using namespace std;

void kkTestCase(CompositeMatchers) {
    kkGroup("both", []() {
        kkTest("both(fitting value, fitting value)", [&]() {
            kkExpect(both(3, 3).matches(3));
        });

        kkTest("both(fitting value, not-fitting value)", [&]() {
            kkExpect(!both(3, 5).matches(3));
        });

        kkTest("both(not-fitting value, fitting value)", [&]() {
            kkExpect(!both(3, 5).matches(5));
        });

        kkTest("both(not-fitting value, not-fitting value)", [&]() {
            kkExpect(!both(3, 5).matches(4));
        });

        kkTest("both(fitting value, fitting matcher)", []() {
            kkExpect(both(3, isLessThan(4)).matches(3));
        });

        kkTest("both(fitting value, un-fitting matcher)", []() {
            kkExpect(!both(3, isLessThan(2)).matches(3));
        });

        kkTest("both(un-fitting value, fitting matcher)", []() {
            kkExpect(!both(3, isLessThan(6)).matches(5));
        });

        kkTest("both(un-fitting value, un-fitting matcher)", []() {
            kkExpect(!both(3, isLessThan(6)).matches(10));
        });

        kkTest("both(fitting matcher, fitting value)", []() {
            kkExpect(both(isLessThan(4), 3).matches(3));
        });

        kkTest("both(un-fitting matcher, fitting value)", []() {
            kkExpect(!both(isLessThan(2), 3).matches(3));
        });

        kkTest("both(fitting matcher, un-fitting value)", []() {
            kkExpect(!both(isLessThan(6), 3).matches(5));
        });

        kkTest("both(un-fitting matcher, un-fitting value)", []() {
            kkExpect(!both(isLessThan(6), 3).matches(10));
        });

        kkTest("both(fitting matcher, fitting matcher)", []() {
            kkExpect(both(isLessThan(7), isGreaterThan(2)).matches(3));
        });

        kkTest("both(fitting matcher, un-fitting matcher)", []() {
            kkExpect(!both(isLessThan(7), isGreaterThan(2)).matches(1));
        });

        kkTest("both(fitting matcher, un-fitting matcher)", []() {
            kkExpect(!both(isLessThan(7), isGreaterThan(2)).matches(10));
        });

        kkTest("both(un-fitting matcher, un-fitting matcher)", []() {
            kkExpect(!both(isLessThan(7), isGreaterThan(20)).matches(10));
        });
    });

    kkGroup("either", []() {
        kkTest("either(fitting value, fitting value)", [&]() {
            kkExpect(either(3, 3).matches(3));
        });

        kkTest("either(fitting value, not-fitting value)", [&]() {
            kkExpect(either(3, 5).matches(3));
        });

        kkTest("either(not-fitting value, fitting value)", [&]() {
            kkExpect(either(3, 5).matches(5));
        });

        kkTest("either(not-fitting value, not-fitting value)", [&]() {
            kkExpect(!either(3, 5).matches(4));
        });

        kkTest("either(fitting value, fitting matcher)", []() {
            kkExpect(either(3, isLessThan(4)).matches(3));
        });

        kkTest("either(fitting value, un-fitting matcher)", []() {
            kkExpect(either(3, isLessThan(2)).matches(3));
        });

        kkTest("either(un-fitting value, fitting matcher)", []() {
            kkExpect(either(3, isLessThan(6)).matches(5));
        });

        kkTest("either(un-fitting value, un-fitting matcher)", []() {
            kkExpect(!either(3, isLessThan(6)).matches(10));
        });

        kkTest("either(fitting matcher, fitting value)", []() {
            kkExpect(either(isLessThan(4), 3).matches(3));
        });

        kkTest("either(un-fitting matcher, fitting value)", []() {
            kkExpect(either(isLessThan(2), 3).matches(3));
        });

        kkTest("either(fitting matcher, un-fitting value)", []() {
            kkExpect(either(isLessThan(6), 3).matches(5));
        });

        kkTest("either(un-fitting matcher, un-fitting value)", []() {
            kkExpect(!either(isLessThan(6), 3).matches(10));
        });

        kkTest("either(fitting matcher, fitting matcher)", []() {
            kkExpect(either(isLessThan(7), isGreaterThan(2)).matches(3));
        });

        kkTest("either(fitting matcher, un-fitting matcher)", []() {
            kkExpect(either(isLessThan(7), isGreaterThan(2)).matches(1));
        });

        kkTest("either(fitting matcher, un-fitting matcher)", []() {
            kkExpect(either(isLessThan(7), isGreaterThan(2)).matches(10));
        });

        kkTest("either(un-fitting matcher, un-fitting matcher)", []() {
            kkExpect(!either(isLessThan(7), isGreaterThan(20)).matches(10));
        });
    });

    kkGroup("isNot", []() {
        kkTest("isNot(fitting value)", []() {
            kkExpect(!isNot(10).matches(10));
        });

        kkTest("isNot(un-fitting value)", []() {
            kkExpect(isNot(5).matches(10));
        });

        kkTest("isNot(fitting matcher)", []() {
            kkExpect(!isNot(isGreaterThan(10)).matches(15));
        });

        kkTest("isNot(un-fitting matcher", []() {
            kkExpect(isNot(isGreaterThan(10)).matches(7));
        });
    });
}
