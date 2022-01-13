#include "mcga/test.hpp"

#include <cstdio>

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::setUp([] {
        printf("first-set-up\n");
    });

    mcga::test::tearDown([] {
        printf("first-tear-down\n");
    });

    mcga::test::group([] {
        mcga::test::setUp([] {
            printf("second-set-up\n");
            mcga::test::fail("fail-in-second-setup");
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

        mcga::test::group([] {
            mcga::test::setUp([] {
                printf("fourth-set-up\n");
            });

            mcga::test::tearDown([] {
                printf("fourth-tear-down\n");
            });

            mcga::test::test("test", [] {
                printf("test-body\n");
            });
        });
    });
};
