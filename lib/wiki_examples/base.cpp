#include <kktest>

void kkTestCase() {
    kkTest("This is a passing test!", [&]() {
        kkExpect(1 + 2 == 3);
    });

    kkTest("This is a failing test!", [&]() {
        kkExpect(3 * 3 == 6);
    });
}
