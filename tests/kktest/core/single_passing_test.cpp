#include <kktest.hpp>

TEST_CASE(singlePassingTest, "Single passing test") {
    kktest::test("This test passes", [] {});
}
