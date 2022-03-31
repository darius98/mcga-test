#include "mcga/test.hpp"

#include <random>

TEST_CASE("TestCase") {
    test({.description = "test", .attempts = 10, .requiredPassedAttempts = 2},
         [] {
             std::random_device device;
             std::default_random_engine engine(device());
             if (std::uniform_int_distribution<int>(0, 4)(engine) == 0) {
                 fail("Got 0.");
             }
         });
}
