#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    test("test", [] {
        fail("1 + 1 == 3 is not true");
    });

    test("test-fail-expect", [] {
        expect(false);
    });
}
