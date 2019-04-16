#include <kktest.hpp>

TEST_CASE(testCase, "TestCase") {
    mcga::test::optionalTest(
      "test", [] { mcga::test::fail("1 + 1 == 3 is not true"); });
}
