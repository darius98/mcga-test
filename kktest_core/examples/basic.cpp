#include <kktest.hpp>

void kkTestCase() {
    kktest::test("This is a passing test!", [] {
        kktest::expect(1 + 2 == 3);
    });

    kktest::test("This is a failing test!", [] {
        kktest::expect(3 * 3 == 6);
    });
}
