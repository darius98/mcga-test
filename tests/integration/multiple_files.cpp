#if MCGA_TEST_INTEGRATION_MULTIPLE_FILES == 1

#include "mcga/test.hpp"

TEST_CASE("TestCase1") {
    mcga::test::test("test", [] {});
}

#elif MCGA_TEST_INTEGRATION_MULTIPLE_FILES == 2

#include "mcga/test.hpp"

TEST_CASE("TestCase2") {
    mcga::test::test("test", [] {});
}

#else

#include "mcga/test.hpp"

TEST_CASE("TestCase3") {
    mcga::test::test("test", [] {});
}

#endif
