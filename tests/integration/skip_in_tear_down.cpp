#include "mcga/test.hpp"

#include <cstdio>

TEST_CASE("TestCase") {
    mcga::test::test("test-1", [] {});

    mcga::test::group([] {
        mcga::test::tearDown([] {
            mcga::test::skip("skip-in-tear-down");
        });

        mcga::test::test("test-2", [] {
            mcga::test::fail("fail-in-test-body");
        });
    });
}
