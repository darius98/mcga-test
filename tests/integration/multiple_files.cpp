#if MCGA_TEST_INTEGRATION_MULTIPLE_FILES == 1

#include "mcga/test.hpp"

static auto t = mcga::test::TestCase{"TestCase1"} + [] {
    mcga::test::test("test", [] {});
};

#elif MCGA_TEST_INTEGRATION_MULTIPLE_FILES == 2

#include "mcga/test.hpp"

static auto t = mcga::test::TestCase{"TestCase2"} + [] {
    mcga::test::test("test", [] {});
};

#else

#include "mcga/test.hpp"

static auto t = mcga::test::TestCase{"TestCase3"} + [] {
    mcga::test::test("test", [] {});
};

#endif
