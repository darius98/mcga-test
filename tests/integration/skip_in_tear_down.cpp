#include "mcga/test.hpp"

#include <cstdio>

TEST_CASE("TestCase") {
    test("test-1", [] {});

    group([] {
        mcga::test::tearDown([] {
            mcga::test::skip("skip-in-tear-down");
        });

        test("test-2", [] {
            fail("fail-in-test-body");
        });
    });
}
