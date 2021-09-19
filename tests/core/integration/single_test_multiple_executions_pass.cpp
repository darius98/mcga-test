#include "mcga/test.hpp"

#include <random>

TEST_CASE("TestCase") {
    mcga::test::test(
      mcga::test::TestConfig("test").setAttempts(100).setRequiredPassedAttempts(
        10),
      [] {
          std::random_device device;
          std::default_random_engine engine(device());
          if (std::uniform_int_distribution<int>(0, 1)(engine)) {
              mcga::test::fail("Got 0.");
          }
      });
}
