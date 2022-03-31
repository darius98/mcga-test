#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    test("test1", [] {});

    test("test2", [] {
        fail("1 + 1 == 3 is not true");
    });

    test("test3", [] {});

    test("test4", [] {});
}
