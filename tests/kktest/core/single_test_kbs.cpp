#include <kktest.hpp>

#include <csignal>

TEST_CASE(testCaseKbs, "Test case kbs") {
    kktest::test("kbs test", [] {
        raise(SIGTERM);
    });
}
