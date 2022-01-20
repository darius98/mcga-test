#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test("test-1", [] {});

    mcga::test::test(mcga::test::TestConfig{.description = "test-2",
                                            .attempts = 10,
                                            .requiredPassedAttempts = 5},
                     [] {
                         mcga::test::skip("every-time");
                     });
};
