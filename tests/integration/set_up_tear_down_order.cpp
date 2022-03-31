#include "mcga/test.hpp"

#include <cstdio>

TEST_CASE("TestCase") {
    mcga::test::setUp([] {
        printf("first-set-up\n");
        mcga::test::cleanup([] {
            printf("set-up-cleanup\n");
        });
    });

    mcga::test::tearDown([] {
        printf("first-tear-down\n");
    });

    group([] {
        mcga::test::setUp([] {
            printf("second-set-up\n");
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

        test("test1", [] {
            mcga::test::cleanup([] {
                printf("first-cleanup\n");
            });

            printf("first-test\n");

            mcga::test::cleanup([] {
                printf("second-cleanup\n");
            });
        });

        test("test2", [] {
            printf("second-test\n");
        });
    });
}
