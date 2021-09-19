#include <thread>

#include "mcga/test.hpp"

using namespace mcga::test;

TEST_CASE("TestCase") {
    // fail() top-level
    fail("fail-top-level");

    group("group", [] {
        // fail() in group
        fail("fail-in-group");

        setUp([] {
            // setUp() in setUp()
            setUp([] {});

            // tearDown() in setUp()
            tearDown([] {});

            // test() in setUp()
            test("test-in-setUp", [] {});

            // group() in setUp()
            group("group-in-setUp", [] {});
        });

        tearDown([] {
            // setUp() in tearDown()
            setUp([] {});

            // tearDown() in tearDown()
            tearDown([] {});

            // test() in tearDown()
            test("test-in-tearDown", [] {});

            // group() in tearDown()
            group("group-in-tearDown", [] {});
        });

        test("test", [] {
            // setUp() in test()
            setUp([] {});

            // tearDown() in test()
            tearDown([] {});

            // test() in test()
            test("test-in-test", [] {});

            // group() in test()
            group("group-in-test", [] {});
        });

        // throw exception in group()
        throw std::runtime_error("exception-in-group");
    });

    group("group2", [] {
        // throw non-exception in group()
        throw 5;
    });

    std::thread t([] {
        // test() in different thread
        test("test-in-thread", [] {});

        // group() in different thread
        group("group-in-thread", [] {});

        // setUp() in different thread
        setUp([] {});

        // tearDown() in different thread
        tearDown([] {});
    });

    t.join();
}
