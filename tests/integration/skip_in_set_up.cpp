#include "mcga/test.hpp"

#include <cstdio>

TEST_CASE("TestCase") {
    test("test-1", [] {});

    group([] {
        mcga::test::setUp([] {
            printf("set-up\n");
            mcga::test::skip("skip-in-set-up");
        });

        mcga::test::tearDown([] {
            printf("tear-down\n");
        });

        test("test-2", [] {
            printf("test-body\n");
            fail("fail-in-test-body");
        });
    });
}
