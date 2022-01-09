#include "mcga/test.hpp"

static auto t = mcga::test::TestCase{"TestCase2"} + [] {
    mcga::test::test("test", [] {});
};
