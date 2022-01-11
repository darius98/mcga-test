#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::setUp([] {
        printf("first-set-up\n");
        mcga::test::cleanup([] {
            printf("set-up-cleanup\n");
        });
    });

    mcga::test::tearDown([] {
        printf("first-tear-down\n");
    });

    mcga::test::group([] {
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

        mcga::test::test("test1", [] {
            mcga::test::cleanup([] {
                printf("first-cleanup\n");
            });

            printf("first-test\n");

            mcga::test::cleanup([] {
                printf("second-cleanup\n");
            });
        });

        mcga::test::test("test2", [] {
            printf("second-test\n");
        });
    });
};
