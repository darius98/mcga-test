#include "mcga/test.hpp"

TEST_CASE(testCase, "TestCase") {
    mcga::test::test("test", [] {
        std::atomic_uint64_t numSpins = 0;
        while (numSpins >= 0) {
            numSpins += 1;
        }
    });
}
