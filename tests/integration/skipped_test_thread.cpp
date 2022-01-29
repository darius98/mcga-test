#include <thread>

#include "mcga/test.hpp"

TEST_CASE("TestCase") {
    mcga::test::test("test-1", [] {});

    mcga::test::test("test-2", [] {
        std::thread t([] {
            mcga::test::skip("In a different thread");
        });

        t.join();
    });
}
