#include "mcga/test.hpp"

#include <cstdio>

TEST_CASE("TestCase") {
    mcga::test::setUp([] {
        printf("set-up\n");
        mcga::test::cleanup([] {
            printf("cleanup-in-set-up\n");
            fail("fail-in-cleanup");
        });
    });

    mcga::test::tearDown([] {
        printf("first-tear-down\n");
    });

    group("group", [] {
        mcga::test::tearDown([] {
            printf("second-tear-down\n");
        });

        test("test", [] {
            printf("test-body\n");
        });
    });
}
