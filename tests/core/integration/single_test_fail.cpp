#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    mcga::test::test("test",
                     [] { mcga::test::fail("1 + 1 == 3 is not true"); });
}
