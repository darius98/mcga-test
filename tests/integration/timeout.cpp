#include "mcga/test.hpp"

#include <atomic>

TEST_CASE("TestCase") {
    test("test", [] {
        std::atomic_uint64_t numSpins = 0;
        while (numSpins >= 0) {
            numSpins += 1;
        }
    });
}
