#include "mcga/test.hpp"

#include <cstdio>

TEST_CASE("TestCase") {
    mcga::test::setUp([] {
        printf("first-set-up\n");
    });

    mcga::test::tearDown([] {
        printf("first-tear-down\n");
    });

    group([] {
        mcga::test::setUp([] {
            printf("second-set-up\n");
            fail("fail-in-second-setup");
        });

        mcga::test::tearDown([] {
            printf("second-tear-down\n");
        });

        mcga::test::tearDown([] {
            printf("third-tear-down\n");
        });

        mcga::test::setUp([] {
            printf("third-set-up\n");
        });

        group([] {
            mcga::test::setUp([] {
                printf("fourth-set-up\n");
            });

            mcga::test::tearDown([] {
                printf("fourth-tear-down\n");
            });

            test("test", [] {
                printf("test-body\n");
            });
        });
    });
}
