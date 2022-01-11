#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test("test", [] {
        mcga::test::skip("skip-this-test");
    });
};
