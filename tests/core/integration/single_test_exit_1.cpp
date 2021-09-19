#include "mcga/test.hpp"

#include <cstdlib>

TEST_CASE("TestCase") {
    mcga::test::test("test", [] { exit(1); });
}
