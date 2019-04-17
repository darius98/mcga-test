#include "mcga/test.hpp"

#include <csignal>

TEST_CASE(testCase, "TestCase") {
    mcga::test::test("test", [] { raise(SIGTERM); });
}
