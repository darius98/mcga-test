#include "mcga/test.hpp"

auto testCase1 = mcga::test::TestCase{"TestCase1"} + [] {
    mcga::test::test("test", [] {});
};

auto testCase2 = mcga::test::TestCase{"TestCase2"} + [] {
    mcga::test::test("test", [] {});
};
