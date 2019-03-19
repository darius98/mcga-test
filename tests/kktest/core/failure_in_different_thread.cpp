#include <thread>

#include <kktest.hpp>

TEST_CASE(failureInDifferentThread, "Failure in different thread") {
    kktest::test("test", [] {
        std::thread t([] {
            kktest::fail("In a different thread");
        });

        t.join();
    });
}
