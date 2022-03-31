#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    // Directly an optional test
    test({.description = "test", .optional = true}, [] {
        fail("1 + 1 == 3 is not true");
    });

    // A test in an optional group
    group({.description = "opt-group", .optional = true}, [] {
        test("non-opt-test", [] {
            fail("1 + 1 == 3 is not true");
        });
    });
}
