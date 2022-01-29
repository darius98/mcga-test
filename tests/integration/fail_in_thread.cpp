#include <thread>

#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    mcga::test::test("test", [] {
        std::thread t([] {
            mcga::test::fail("In a different thread");
        });

        t.join();
    });
}
