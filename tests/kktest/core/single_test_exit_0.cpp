#include <kktest.hpp>

#include <cstdlib>

TEST_CASE(singleTestExit0, "Single test exit 0") {
    kktest::test("This test exits with code 0", [] {
        exit(0);
    });
}
