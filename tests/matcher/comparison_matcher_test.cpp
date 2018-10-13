#include <kktest_matchers>
#include <kktest>

using namespace kktest;
using namespace std;

void testCase() {
    group("Equality", [&]() {
        test("isEqualTo matches equal values", [&]() {
            expect(isEqualTo(3)->matches(3));
            expect(isEqualTo(vector<int>{1,2})->matches(vector<int>{1,2}));
        });

        test("isEqualTo fails for different values", [&]() {
            expect(!isEqualTo(3)->matches(2));
            expect(!isEqualTo(2)->matches(3));
            expect(
                !isEqualTo(vector<int>{1,3})->matches(vector<int>{2,3}));
        });

        test("Description for isEqualTo on failing values", [&]() {
            Description description, mismatchDescription;
            auto m = isEqualTo(2);
            m->matches(3);
            m->describe(description);
            m->describeMismatch(mismatchDescription);
            expect(description.toString() == "'2'");
            expect(mismatchDescription.toString() == "not '2'");
        });
    });

    group("Identity", [&]() {
        test("isIdenticalTo matches identical variables", [&]() {
            int x = 4;
            expect(isIdenticalTo(x)->matches(x));
        });

        test("isIdenticalTo fails for equal values", [&]() {
            int x = 3, y = 3;
            expect(!isIdenticalTo(x)->matches(y));
            expect(!isIdenticalTo(y)->matches(x));
        });

        test("isIdenticalTo fails for different values", [&]() {
            int x = 3, y = 5;
            expect(!isIdenticalTo(x)->matches(y));
            expect(!isIdenticalTo(y)->matches(x));
        });
    });

    group("Less than", [&]() {
        test("isLessThan matches smaller values", [&]() {
            expect(isLessThan(5)->matches(3));
            expect(isLessThan(17)->matches(-20));
            expect(isLessThan('c')->matches('a'));
            expect(isLessThan(string("cadabra"))->matches("abra"));
            expect(isLessThan(vector<int>{2, 1, 3})
                       ->matches(vector<int>{1, 2, 3}));
        });

        test("isLessThan does not match equal values", [&]() {
            expect(!isLessThan(3)->matches(3));
            expect(!isLessThan(-17)->matches(-17));
            expect(!isLessThan('c')->matches('c'));
            expect(!isLessThan(string("cadabra"))->matches("cadabra"));
            expect(!isLessThan(vector<int>{2, 1, 3})
                       ->matches(vector<int>{2, 1, 3}));
        });

        test("isLessThan does not match greater values", [&]() {
            expect(!isLessThan(3)->matches(5));
            expect(!isLessThan(-20)->matches(17));
            expect(!isLessThan('a')->matches('c'));
            expect(!isLessThan(string("abra"))->matches("cadabra"));
            expect(!isLessThan(vector<int>{1, 2, 3})
                       ->matches(vector<int>{2, 1, 3}));
        });

        test("Failure description for isLessThan", [&]() {
            Description description, mismatchDescription;
            auto m = isLessThan(vector<int>{2,1,3});
            m->matches(vector<int>{3,1,2});
            m->describe(description);
            m->describeMismatch(mismatchDescription);
            expect(description.toString() == "< '[2, 1, 3]'");
            expect(mismatchDescription.toString() == "not < '[2, 1, 3]'");
        });
    });

    group("Less than or equal to", [&]() {
        test("isLessThanOrEqualTo matches smaller values", [&]() {
            expect(isLessThanOrEqualTo(5)->matches(3));
            expect(isLessThanOrEqualTo(17)->matches(-20));
            expect(isLessThanOrEqualTo('c')->matches('a'));
            expect(isLessThanOrEqualTo(string("cada"))->matches("abra"));
            expect(isLessThanOrEqualTo(vector<int>{2, 1, 3})
                       ->matches(vector<int>{1, 2, 3}));
        });

        test("isLessThanOrEqualTo matches equal values", [&]() {
            expect(isLessThanOrEqualTo(3)->matches(3));
            expect(isLessThanOrEqualTo(-17)->matches(-17));
            expect(isLessThanOrEqualTo('c')->matches('c'));
            expect(isLessThanOrEqualTo(string("cada"))->matches("cada"));
            expect(isLessThanOrEqualTo(vector<int>{2, 1, 3})
                       ->matches(vector<int>{2, 1, 3}));
        });

        test("isLessThanOrEqualTo does not match greater values", [&]() {
            expect(!isLessThanOrEqualTo(3)->matches(5));
            expect(!isLessThanOrEqualTo(-20)->matches(17));
            expect(!isLessThanOrEqualTo('a')->matches('c'));
            expect(!isLessThanOrEqualTo(string("abra"))->matches("cada"));
            expect(!isLessThanOrEqualTo(vector<int>{1, 2, 3})
                       ->matches(vector<int>{2, 1, 3}));
        });

        test("Failure description for isLessThanOrEqualTo", [&]() {
            Description description, mismatchDescription;
            auto m = isLessThanOrEqualTo(vector<int>{2,1,3});
            m->matches(vector<int>{3,1,2});
            m->describe(description);
            m->describeMismatch(mismatchDescription);
            expect(description.toString() == "<= '[2, 1, 3]'");
            expect(mismatchDescription.toString() == "not <= '[2, 1, 3]'");
        });
    });

    group("Greater than", [&]() {
        test("isGreaterThan does not match smaller values", [&]() {
            expect(!isGreaterThan(5)->matches(3));
            expect(!isGreaterThan(17)->matches(-20));
            expect(!isGreaterThan('c')->matches('a'));
            expect(!isGreaterThan(string("cadabra"))->matches("abra"));
            expect(!isGreaterThan(vector<int>{2, 1, 3})
                       ->matches(vector<int>{1, 2, 3}));
        });

        test("isGreaterThan does not match equal values", [&]() {
            expect(!isGreaterThan(3)->matches(3));
            expect(!isGreaterThan(-17)->matches(-17));
            expect(!isGreaterThan('c')->matches('c'));
            expect(!isGreaterThan(string("cadabra"))->matches("cadabra"));
            expect(!isGreaterThan(vector<int>{2, 1, 3})
                       ->matches(vector<int>{2, 1, 3}));
        });

        test("isGreaterThan matches greater values", [&]() {
            expect(isGreaterThan(3)->matches(5));
            expect(isGreaterThan(-20)->matches(17));
            expect(isGreaterThan('a')->matches('c'));
            expect(isGreaterThan(string("abra"))->matches("cadabra"));
            expect(isGreaterThan(vector<int>{1, 2, 3})
                       ->matches(vector<int>{2, 1, 3}));
        });

        test("Failure description for isGreaterThan", [&]() {
            Description description, mismatchDescription;
            auto m = isGreaterThan(vector<int>{2,1,3});
            m->matches(vector<int>{1,2,3});
            m->describe(description);
            m->describeMismatch(mismatchDescription);
            expect(description.toString() == "> '[2, 1, 3]'");
            expect(mismatchDescription.toString() == "not > '[2, 1, 3]'");
        });
    });

    group("Greater than or equal to", [&]() {
        test("isGreaterThanOrEqualTo does not match smaller values", [&]() {
            expect(!isGreaterThanOrEqualTo(5)->matches(3));
            expect(!isGreaterThanOrEqualTo(17)->matches(-20));
            expect(!isGreaterThanOrEqualTo('c')->matches('a'));
            expect(!isGreaterThanOrEqualTo(string("cada"))->matches("abra"));
            expect(!isGreaterThanOrEqualTo(vector<int>{2, 1, 3})
                       ->matches(vector<int>{1, 2, 3}));
        });

        test("isGreaterThanOrEqualTo matches equal values", [&]() {
            expect(isGreaterThanOrEqualTo(3)->matches(3));
            expect(isGreaterThanOrEqualTo(-17)->matches(-17));
            expect(isGreaterThanOrEqualTo('c')->matches('c'));
            expect(isGreaterThanOrEqualTo(string("cada"))->matches("cada"));
            expect(isGreaterThanOrEqualTo(vector<int>{2, 1, 3})
                       ->matches(vector<int>{2, 1, 3}));
        });

        test("isGreaterThanOrEqualTo matches greater values", [&]() {
            expect(isGreaterThanOrEqualTo(3)->matches(5));
            expect(isGreaterThanOrEqualTo(-20)->matches(17));
            expect(isGreaterThanOrEqualTo('a')->matches('c'));
            expect(isGreaterThanOrEqualTo(string("abra"))->matches("cada"));
            expect(isGreaterThanOrEqualTo(vector<int>{1, 2, 3})
                       ->matches(vector<int>{2, 1, 3}));
        });

        test("Failure description for isGreaterThanOrEqualTo", [&]() {
            Description description, mismatchDescription;
            auto m = isGreaterThanOrEqualTo(vector<int>{2,1,3});
            m->matches(vector<int>{1,2,3});
            m->describe(description);
            m->describeMismatch(mismatchDescription);
            expect(description.toString() == ">= '[2, 1, 3]'");
            expect(mismatchDescription.toString() == "not >= '[2, 1, 3]'");
        });
    });
}
