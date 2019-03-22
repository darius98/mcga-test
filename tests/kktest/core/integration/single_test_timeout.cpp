#include <kktest.hpp>

TEST_CASE(testCase, "TestCase") {
    kktest::test("test", [] {
        volatile unsigned long long numSpins = 0;
        while (numSpins >= 0) {
            numSpins += 1;
        }
    });
}
