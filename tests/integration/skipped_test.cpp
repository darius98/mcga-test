#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    mcga::test::test("test-1", [] {});

    mcga::test::test("test-2", [] {
        mcga::test::skip("skip-this-test");
    });
}
