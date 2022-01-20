#include "mcga/test.hpp"

#include <random>

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test(
      {.description = "test", .attempts = 10, .requiredPassedAttempts = 2}, [] {
          std::random_device device;
          std::default_random_engine engine(device());
          if (std::uniform_int_distribution<int>(0, 4)(engine) == 0) {
              mcga::test::fail("Got 0.");
          }
      });
};
