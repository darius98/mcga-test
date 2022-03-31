#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    test("test-1", [] {});

    test({.description = "test-2", .attempts = 10, .requiredPassedAttempts = 5},
         [] {
             mcga::test::skip("every-time");
         });
}
