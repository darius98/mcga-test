#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test("test", [] {
        mcga::test::fail("1 + 1 == 3 is not true");
    });
};
