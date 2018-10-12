#include <kktest_matchers>
#include <kktest>

using namespace kktest;

void testCase() {
    test("isTrue matcher matches true", [&]() {
        expect(isTrue->matches(true));
    });

    test("isTrue matcher does not match false", [&]() {
        expect(!isTrue->matches(false));
    });

    test("isTrue description is 'true'", [&]() {
        Description description;
        isTrue->describe(description);
        expect(description.toString() == "true");
    });

    test("isTrue mismatch description is 'false'", [&]() {
        Description description;
        isTrue->describeMismatch(description);
        expect(description.toString() == "false");
    });

    test("isFalse matcher does not match true", [&]() {
        expect(!isFalse->matches(true));
    });

    test("isFalse matcher matches false", [&]() {
        expect(isFalse->matches(false));
    });

    test("isFalse description is 'false'", [&]() {
        Description description;
        isFalse->describe(description);
        expect(description.toString() == "false");
    });

    test("isFalse mismatch description is 'true'", [&]() {
        Description description;
        isFalse->describeMismatch(description);
        expect(description.toString() == "true");
    });
}
