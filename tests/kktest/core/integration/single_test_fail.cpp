#include <kktest.hpp>

TEST_CASE(testCase, "TestCase") {
    kktest::test("test", [] { kktest::fail("1 + 1 == 3 is not true"); });
}
