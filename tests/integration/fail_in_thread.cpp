#include <thread>

#include "mcga/test.hpp"

auto testCase = mcga::test::TestCase{"TestCase"} + [] {
    mcga::test::test("test", [] {
        std::thread t([] {
            mcga::test::fail("In a different thread");
        });

        t.join();
    });
};
