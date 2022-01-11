#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test(
      mcga::test::TestConfig{
        .description = "test", .attempts = 100, .requiredPassedAttempts = 50},
      [] {
          mcga::test::skip("every-time");
      });
};
