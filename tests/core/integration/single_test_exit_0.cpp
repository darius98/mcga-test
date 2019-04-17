#include "mcga/test.hpp"

#include <cstdlib>

TEST_CASE(testCase, "TestCase") {
    mcga::test::test("test", [] { exit(0); });
}
