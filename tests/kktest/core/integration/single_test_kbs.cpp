#include <kktest.hpp>

#include <csignal>

TEST_CASE(testCase, "TestCase") {
    kktest::test("test", [] { raise(SIGTERM); });
}
