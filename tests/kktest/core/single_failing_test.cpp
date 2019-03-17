#include <kktest.hpp>

TEST_CASE(singleFailingTest, "Single failing test") {
    kktest::test("This test fails", [] {
        kktest::fail("1 + 1 == 3 is not true");
    });
}
