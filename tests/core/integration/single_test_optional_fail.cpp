#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    // Directly an optional test
    mcga::test::optionalTest(
      "test", [] { mcga::test::fail("1 + 1 == 3 is not true"); });

    // A test in an optional group
    mcga::test::optionalGroup("optional-group", [] {
        mcga::test::test("non-optional-test",
                         [] { mcga::test::fail("1 + 1 == 3 is not true"); });
    });
}
