#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test("test-1", [] {});

    mcga::test::test("test-2", [] {
        mcga::test::skip("skip-this-test");
    });
};
