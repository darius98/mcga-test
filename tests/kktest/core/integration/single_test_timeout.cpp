#include <cstdint>

#include <kktest.hpp>

TEST_CASE(testCase, "TestCase") {
    kktest::test("test", [] {
        volatile std::uint64_t numSpins = 0;
        while (numSpins >= 0) {
            numSpins += 1;
        }
    });
}
