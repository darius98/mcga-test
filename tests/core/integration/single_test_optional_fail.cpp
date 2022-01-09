#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    // Directly an optional test
    mcga::test::test.optional("test", [] {
        mcga::test::fail("1 + 1 == 3 is not true");
    });

    // A test in an optional group
    mcga::test::group.optional("optional-group", [] {
        mcga::test::test("non-optional-test", [] {
            mcga::test::fail("1 + 1 == 3 is not true");
        });
    });
};
