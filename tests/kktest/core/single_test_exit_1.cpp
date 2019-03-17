#include <kktest.hpp>

#include <cstdlib>

TEST_CASE(singleTestExit1, "Single test exit 1") {
    kktest::test("This test exits with code 1", [] {
        exit(1);
    });
}
