#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    // Directly an optional test
    mcga::test::test({.description = "test", .optional = true}, [] {
        mcga::test::fail("1 + 1 == 3 is not true");
    });

    // A test in an optional group
    mcga::test::group({.description = "opt-group", .optional = true}, [] {
        mcga::test::test("non-opt-test", [] {
            mcga::test::fail("1 + 1 == 3 is not true");
        });
    });
}
