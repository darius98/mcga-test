#include <cstdint>

#include "mcga/test.hpp"

TEST_CASE(testCase, "TestCase") {
    mcga::test::test("test", [] {
        volatile std::uint64_t numSpins = 0;
        while (numSpins >= 0) {
            numSpins += 1;
        }
    });
}
