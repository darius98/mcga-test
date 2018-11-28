#include <kktest>
#include <kktest_ext/core_matchers>

using namespace kktest::core_matchers;

void kkTestCase(TruthMatchers) {
    kkTest("isTrue matcher matches true", [&]() {
        kkExpect(isTrue.matches(true));
    });

    kkTest("isTrue matcher does not match false", [&]() {
        kkExpect(!isTrue.matches(false));
    });

    kkTest("isTrue description is 'true'", [&]() {
        Description description;
        isTrue.describe(description);
        kkExpect(description.toString() == "true");
    });

    kkTest("isTrue mismatch description is 'false'", [&]() {
        Description description;
        isTrue.describeMismatch(description);
        kkExpect(description.toString() == "false");
    });

    kkTest("isFalse matcher does not match true", [&]() {
        kkExpect(!isFalse.matches(true));
    });

    kkTest("isFalse matcher matches false", [&]() {
        kkExpect(isFalse.matches(false));
    });

    kkTest("isFalse description is 'false'", [&]() {
        Description description;
        isFalse.describe(description);
        kkExpect(description.toString() == "false");
    });

    kkTest("isFalse mismatch description is 'true'", [&]() {
        Description description;
        isFalse.describeMismatch(description);
        kkExpect(description.toString() == "true");
    });
}
