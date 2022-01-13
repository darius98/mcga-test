#include "mcga/test.hpp"

static auto t = mcga::test::TestCase{"TestCase1"} + [] {
    mcga::test::test("test", [] {});
};
