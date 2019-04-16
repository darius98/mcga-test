#include <kktest.hpp>

TEST_CASE(testCase, "TestCase") {
    kktest::test("test1", [] {});

    kktest::test("test2", [] { kktest::fail("1 + 1 == 3 is not true"); });

    kktest::test("test3", [] {});

    kktest::test("test4", [] {});
}
