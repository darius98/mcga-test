#include <kktest.hpp>

#include <cstdlib>

TEST_CASE(testCase, "TestCase") {
    kktest::test("test", [] {
        exit(1);
    });
}
