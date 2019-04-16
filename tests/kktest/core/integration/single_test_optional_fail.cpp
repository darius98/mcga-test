#include <kktest.hpp>

TEST_CASE(testCase, "TestCase") {
    kktest::optionalTest("test",
                         [] { kktest::fail("1 + 1 == 3 is not true"); });
}
