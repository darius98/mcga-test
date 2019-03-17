#include <kktest.hpp>

TEST_CASE(testCaseKbs, "Test case timeout") {
    kktest::test("timeout test", [] {
        volatile unsigned long long numSpins = 0;
        while (numSpins >= 0) {
            numSpins += 1;
        }
    });
}
