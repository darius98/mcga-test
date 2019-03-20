#include <kktest.hpp>

#include <random>

TEST_CASE(testCase, "TestCase") {
    kktest::test(kktest::TestConfig("test")
                     .setAttempts(100)
                     .setRequiredPassedAttempts(10), [] {
         std::random_device device;
         std::default_random_engine engine(device());
        if (std::uniform_int_distribution<int>(0, 1)(engine)) {
            kktest::fail("Got 0.");
        }
    });
}
