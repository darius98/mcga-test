#include <kktest_matchers>
#include <kktest>

using namespace kktest;
using namespace std;

void testCase() {
    group("Equality", [&]() {
        test("isEqualTo(3)->matches(3)", [&]() {
            expect(isEqualTo(3)->matches(3));
        });

        test("isEqualTo(vector{1,2,3})->matches(vector{1,2,3})", [&]() {
            expect(isEqualTo(vector<int>{1,2,3})->matches(vector<int>{1,2,3}));
        });

        test("!isEqualTo(3)->matches(2)", [&]() {
            expect(!isEqualTo(3)->matches(2));
        });

        test("!isEqualTo(2)->matches(3)", [&]() {
            expect(!isEqualTo(2)->matches(3));
        });

        test("Description for isEqualTo(2) failing", [&]() {
            Description description, mismatchDescription;
            auto m = isEqualTo(2);
            m->matches(3);
            m->describe(description);
            m->describeMismatch(mismatchDescription);
            expect(description.toString() == "'2'");
            expect(mismatchDescription.toString() == "not '2'");
        });
    });
}
