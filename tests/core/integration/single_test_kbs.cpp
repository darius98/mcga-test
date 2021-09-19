#include "mcga/test.hpp"

#include <csignal>

TEST_CASE("TestCase") {
    mcga::test::test("test", [] { raise(SIGTERM); });
}
