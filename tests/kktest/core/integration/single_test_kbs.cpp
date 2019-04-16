#include <kktest.hpp>

#include <csignal>

TEST_CASE(testCase, "TestCase") {
    mcga::test::test("test", [] { raise(SIGTERM); });
}
