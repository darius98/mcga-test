#include "mcga/test.hpp"

#include <cstdlib>

TEST_CASE("TestCase") {
    test("test", [] {
        exit(1);
    });
}
