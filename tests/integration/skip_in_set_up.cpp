#include "mcga/test.hpp"

#include <cstdio>

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test("test-1", [] {});

    mcga::test::group([] {
        mcga::test::setUp([] {
            printf("set-up\n");
            mcga::test::skip("skip-in-set-up");
        });

        mcga::test::tearDown([] {
            printf("tear-down\n");
        });

        mcga::test::test("test-2", [] {
            printf("test-body\n");
            mcga::test::fail("fail-in-test-body");
        });
    });
};
