#include <kktest>
#include <kktest_matchers>

using namespace kktest;
using namespace std;

void kkTestCase() {
    kkGroup("Equality", [&]() {
        kkTest("isEqualTo matches equal values", [&]() {
            kkExpect(isEqualTo(3).matches(3));
            kkExpect(isEqualTo(vector<int>{1,2}).matches(vector<int>{1,2}));
        });

        kkTest("isEqualTo fails for different values", [&]() {
            kkExpect(!isEqualTo(3).matches(2));
            kkExpect(!isEqualTo(2).matches(3));
            kkExpect(!isEqualTo(vector<int>{1,3}).matches(vector<int>{2,3}));
        });

        kkTest("Description for isEqualTo on failing values", [&]() {
            Description description, mismatchDescription;
            auto m = isEqualTo(2);
            m.matches(3);
            m.describe(description);
            m.describeMismatch(mismatchDescription);
            kkExpect(description.toString() == "'2'");
            kkExpect(mismatchDescription.toString() == "not '2'");
        });

        kkTest("isNotEqualTo does not match equal values", [&]() {
            kkExpect(!isNotEqualTo(3).matches(3));
            kkExpect(!isNotEqualTo(vector<int>{1, 2}).matches(vector<int>{1, 2}));
        });

        kkTest("isNotEqualTo matches for different values", [&]() {
            kkExpect(isNotEqualTo(3).matches(2));
            kkExpect(isNotEqualTo(2).matches(3));
            kkExpect(isNotEqualTo(vector<int>{1, 3}).matches(vector<int>{2, 3}));
        });

        kkTest("Description for isNotEqualTo on equal values", [&]() {
            Description description, mismatchDescription;
            auto m = isNotEqualTo(2);
            m.matches(2);
            m.describe(description);
            m.describeMismatch(mismatchDescription);
            kkExpect(description.toString() == "different than '2'");
            kkExpect(mismatchDescription.toString() == "not different than '2'");
        });
    });

    kkGroup("Identity", [&]() {
        kkTest("isIdenticalTo matches identical variables", [&]() {
            int x = 4;
            kkExpect(isIdenticalTo(x).matches(x));
        });

        kkTest("isIdenticalTo fails for equal values", [&]() {
            int x = 3, y = 3;
            kkExpect(!isIdenticalTo(x).matches(y));
            kkExpect(!isIdenticalTo(y).matches(x));
        });

        kkTest("isIdenticalTo fails for different values", [&]() {
            int x = 3, y = 5;
            kkExpect(!isIdenticalTo(x).matches(y));
            kkExpect(!isIdenticalTo(y).matches(x));
        });
    });

    kkGroup("Less than", [&]() {
        kkTest("isLessThan matches smaller values", [&]() {
            kkExpect(isLessThan(5).matches(3));
            kkExpect(isLessThan(17).matches(-20));
            kkExpect(isLessThan('c').matches('a'));
            kkExpect(isLessThan(string("cadabra")).matches("abra"));
            kkExpect(isLessThan(vector<int>{2, 1, 3}).matches(vector<int>{1, 2, 3}));
        });

        kkTest("isLessThan does not match equal values", [&]() {
            kkExpect(!isLessThan(3).matches(3));
            kkExpect(!isLessThan(-17).matches(-17));
            kkExpect(!isLessThan('c').matches('c'));
            kkExpect(!isLessThan(string("cadabra")).matches("cadabra"));
            kkExpect(!isLessThan(vector<int>{2, 1, 3}).matches(vector<int>{2, 1, 3}));
        });

        kkTest("isLessThan does not match greater values", [&]() {
            kkExpect(!isLessThan(3).matches(5));
            kkExpect(!isLessThan(-20).matches(17));
            kkExpect(!isLessThan('a').matches('c'));
            kkExpect(!isLessThan(string("abra")).matches("cadabra"));
            kkExpect(!isLessThan(vector<int>{1, 2, 3}).matches(vector<int>{2, 1, 3}));
        });

        kkTest("Failure description for isLessThan", [&]() {
            Description description, mismatchDescription;
            auto m = isLessThan(vector<int>{2,1,3});
            m.matches(vector<int>{3,1,2});
            m.describe(description);
            m.describeMismatch(mismatchDescription);
            kkExpect(description.toString() == "< '[2, 1, 3]'");
            kkExpect(mismatchDescription.toString() == "not < '[2, 1, 3]'");
        });
    });

    kkGroup("Less than or equal to", [&]() {
        kkTest("isLessThanEqual matches smaller values", [&]() {
            kkExpect(isLessThanEqual(5).matches(3));
            kkExpect(isLessThanEqual(17).matches(-20));
            kkExpect(isLessThanEqual('c').matches('a'));
            kkExpect(isLessThanEqual(string("cada")).matches("abra"));
            kkExpect(isLessThanEqual(vector<int>{2, 1, 3}).matches(vector<int>{1, 2, 3}));
        });

        kkTest("isLessThanEqual matches equal values", [&]() {
            kkExpect(isLessThanEqual(3).matches(3));
            kkExpect(isLessThanEqual(-17).matches(-17));
            kkExpect(isLessThanEqual('c').matches('c'));
            kkExpect(isLessThanEqual(string("cada")).matches("cada"));
            kkExpect(isLessThanEqual(vector<int>{2, 1, 3}).matches(vector<int>{2, 1, 3}));
        });

        kkTest("isLessThanEqual does not match greater values", [&]() {
            kkExpect(!isLessThanEqual(3).matches(5));
            kkExpect(!isLessThanEqual(-20).matches(17));
            kkExpect(!isLessThanEqual('a').matches('c'));
            kkExpect(!isLessThanEqual(string("abra")).matches("cada"));
            kkExpect(!isLessThanEqual(vector<int>{1, 2, 3}).matches(vector<int>{2, 1, 3}));
        });

        kkTest("Failure description for isLessThanEqual", [&]() {
            Description description, mismatchDescription;
            auto m = isLessThanEqual(vector<int>{2,1,3});
            m.matches(vector<int>{3,1,2});
            m.describe(description);
            m.describeMismatch(mismatchDescription);
            kkExpect(description.toString() == "<= '[2, 1, 3]'");
            kkExpect(mismatchDescription.toString() == "not <= '[2, 1, 3]'");
        });
    });

    kkGroup("Greater than", [&]() {
        kkTest("isGreaterThan does not match smaller values", [&]() {
            kkExpect(!isGreaterThan(5).matches(3));
            kkExpect(!isGreaterThan(17).matches(-20));
            kkExpect(!isGreaterThan('c').matches('a'));
            kkExpect(!isGreaterThan(string("cadabra")).matches("abra"));
            kkExpect(!isGreaterThan(vector<int>{2, 1, 3}).matches(vector<int>{1, 2, 3}));
        });

        kkTest("isGreaterThan does not match equal values", [&]() {
            kkExpect(!isGreaterThan(3).matches(3));
            kkExpect(!isGreaterThan(-17).matches(-17));
            kkExpect(!isGreaterThan('c').matches('c'));
            kkExpect(!isGreaterThan(string("cadabra")).matches("cadabra"));
            kkExpect(!isGreaterThan(vector<int>{2, 1, 3}).matches(vector<int>{2, 1, 3}));
        });

        kkTest("isGreaterThan matches greater values", [&]() {
            kkExpect(isGreaterThan(3).matches(5));
            kkExpect(isGreaterThan(-20).matches(17));
            kkExpect(isGreaterThan('a').matches('c'));
            kkExpect(isGreaterThan(string("abra")).matches("cadabra"));
            kkExpect(isGreaterThan(vector<int>{1, 2, 3}).matches(vector<int>{2, 1, 3}));
        });

        kkTest("Failure description for isGreaterThan", [&]() {
            Description description, mismatchDescription;
            auto m = isGreaterThan(vector<int>{2,1,3});
            m.matches(vector<int>{1,2,3});
            m.describe(description);
            m.describeMismatch(mismatchDescription);
            kkExpect(description.toString() == "> '[2, 1, 3]'");
            kkExpect(mismatchDescription.toString() == "not > '[2, 1, 3]'");
        });
    });

    kkGroup("Greater than or equal to", [&]() {
        kkTest("isGreaterThanEqual does not match smaller values", [&]() {
            kkExpect(!isGreaterThanEqual(5).matches(3));
            kkExpect(!isGreaterThanEqual(17).matches(-20));
            kkExpect(!isGreaterThanEqual('c').matches('a'));
            kkExpect(!isGreaterThanEqual(string("cada")).matches("abra"));
            kkExpect(!isGreaterThanEqual(vector<int>{2, 1, 3}).matches(vector<int>{1, 2, 3}));
        });

        kkTest("isGreaterThanEqual matches equal values", [&]() {
            kkExpect(isGreaterThanEqual(3).matches(3));
            kkExpect(isGreaterThanEqual(-17).matches(-17));
            kkExpect(isGreaterThanEqual('c').matches('c'));
            kkExpect(isGreaterThanEqual(string("cada")).matches("cada"));
            kkExpect(isGreaterThanEqual(vector<int>{2, 1, 3}).matches(vector<int>{2, 1, 3}));
        });

        kkTest("isGreaterThanEqual matches greater values", [&]() {
            kkExpect(isGreaterThanEqual(3).matches(5));
            kkExpect(isGreaterThanEqual(-20).matches(17));
            kkExpect(isGreaterThanEqual('a').matches('c'));
            kkExpect(isGreaterThanEqual(string("abra")).matches("cada"));
            kkExpect(isGreaterThanEqual(vector<int>{1, 2, 3}).matches(vector<int>{2, 1, 3}));
        });

        kkTest("Failure description for isGreaterThanEqual", [&]() {
            Description description, mismatchDescription;
            auto m = isGreaterThanEqual(vector<int>{2,1,3});
            m.matches(vector<int>{1,2,3});
            m.describe(description);
            m.describeMismatch(mismatchDescription);
            kkExpect(description.toString() == ">= '[2, 1, 3]'");
            kkExpect(mismatchDescription.toString() == "not >= '[2, 1, 3]'");
        });
    });
}
