#include "mcga/test.hpp"

#include <random>

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test(
      mcga::test::TestConfig{
        .description = "test", .attempts = 10, .requiredPassedAttempts = 5},
      [] {
          std::random_device device;
          std::default_random_engine engine(device());
          if (std::uniform_int_distribution<int>(0, 100)(engine)) {
              mcga::test::fail("Got 0.");
          }
      });
};
