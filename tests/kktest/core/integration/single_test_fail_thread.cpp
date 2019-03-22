#include <thread>

#include <kktest.hpp>

TEST_CASE(testCase, "TestCase") {
    kktest::test("test", [] {
        std::thread t([] {
            kktest::fail("In a different thread");
        });

        t.join();
    });
}
