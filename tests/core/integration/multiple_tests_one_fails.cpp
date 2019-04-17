#include "mcga/test.hpp"

TEST_CASE(testCase, "TestCase") {
    mcga::test::test("test1", [] {});

    mcga::test::test("test2",
                     [] { mcga::test::fail("1 + 1 == 3 is not true"); });

    mcga::test::test("test3", [] {});

    mcga::test::test("test4", [] {});
}
