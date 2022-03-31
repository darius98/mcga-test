#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    test("test-1", [] {});

    test("test-2", [] {
        mcga::test::skip("skip-this-test");
    });
}
