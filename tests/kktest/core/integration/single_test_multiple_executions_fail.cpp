#include <kktest.hpp>

#include <random>

TEST_CASE(testCase, "TestCase") {
    mcga::test::test(
      mcga::test::TestConfig("test").setAttempts(100).setRequiredPassedAttempts(
        50),
      [] {
          std::random_device device;
          std::default_random_engine engine(device());
          if (std::uniform_int_distribution<int>(0, 100)(engine)) {
              mcga::test::fail("Got 0.");
          }
      });
}
