#include "mcga/test.hpp"

#include <csignal>

TEST_CASE("TestCase") {
    test("test", [] {
        raise(SIGTERM);
    });
}
