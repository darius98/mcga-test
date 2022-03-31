#include <thread>

#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    test("test", [] {
        std::thread t([] {
            fail("In a different thread");
        });

        t.join();
    });
}
