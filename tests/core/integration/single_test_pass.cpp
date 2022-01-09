#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test("test", [] {});
};
