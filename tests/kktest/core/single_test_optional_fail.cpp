#include <kktest.hpp>

TEST_CASE(testCase, "TestCase") {
    kktest::test(kktest::TestConfig("test").setOptional(), [] {
        kktest::fail("1 + 1 == 3 is not true");
    });
}
