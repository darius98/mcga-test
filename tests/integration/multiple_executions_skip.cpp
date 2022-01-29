#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    mcga::test::test("test-1", [] {});

    mcga::test::test(
      {.description = "test-2", .attempts = 10, .requiredPassedAttempts = 5},
      [] {
          mcga::test::skip("every-time");
      });
}
