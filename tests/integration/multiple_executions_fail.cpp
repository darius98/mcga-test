#include "mcga/test.hpp"

#include <random>

TEST_CASE("TestCase") {
    test({.description = "test", .attempts = 10, .requiredPassedAttempts = 5},
         [] {
             std::random_device device;
             std::default_random_engine engine(device());
             if (std::uniform_int_distribution<int>(0, 100)(engine)) {
                 fail("Got 0.");
             }
         });
}
