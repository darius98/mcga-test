#include "mcga/test.hpp"

#include <cstdlib>

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test("test", [] { exit(0); });
};
