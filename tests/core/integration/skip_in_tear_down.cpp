#include "mcga/test.hpp"

#include <cstdio>

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::tearDown([] {
        mcga::test::skip("skip-in-tear-down");
    });

    mcga::test::test("test", [] {
        mcga::test::fail("fail-in-test-body");
    });
};
