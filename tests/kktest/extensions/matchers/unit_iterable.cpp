#include <vector>

#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers/iterable.hpp"
#include "kktest/extensions/matchers/include/kktest_ext/matchers/numeric.hpp"
#include "kktest/extensions/matchers/include/kktest_ext/matchers/testing_helpers.hpp"

using namespace mcga::test;
using namespace mcga::test::matchers;
using namespace std;

TEST_CASE(iterable, "Matchers extension: iterable") {
    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15};
    vector<vector<int>> matrix = {{1, 2, 3, 4}, {2, 3, 4}, {2, 3}, {2, 4}};
    vector<int> emptyArr = {};
    vector<int> all3Arr = {3, 3, 3, 3, 3};

    test("isEmpty matches empty array",
         [&] { EXPECT_MATCHER_MATCHES(emptyArr, isEmpty); });

    test("isEmpty does not match non-empty arrays", [&] {
        EXPECT_MATCHER_FAILS(arr, isEmpty);
        EXPECT_MATCHER_FAILS(matrix, isEmpty);
    });

    test("isNotEmpty does not match empty array",
         [&] { EXPECT_MATCHER_FAILS(emptyArr, isNotEmpty); });

    test("isNotEmpty matches non-empty arrays", [&] {
        EXPECT_MATCHER_MATCHES(arr, isNotEmpty);
        EXPECT_MATCHER_MATCHES(matrix, isNotEmpty);
    });

    test("hasSize(8) matches iterable with size 8",
         [&] { EXPECT_MATCHER_MATCHES(arr, hasSize(8)); });

    test("hasSize(5) does not match iterable with size 8",
         [&] { EXPECT_MATCHER_FAILS(arr, hasSize(5)); });

    test("hasSize works with size sub-matcher", [&] {
        EXPECT_MATCHER_MATCHES(arr, hasSize(isLessThan(10)));
        EXPECT_MATCHER_MATCHES(arr, hasSize(isGreaterThan(7)));
        EXPECT_MATCHER_MATCHES(arr, hasSize(isEven));
        EXPECT_MATCHER_FAILS(arr, hasSize(isOdd));
        EXPECT_MATCHER_FAILS(arr, hasSize(isGreaterThan(20)));
    });

    test("eachElement(3) matches empty array",
         [&] { EXPECT_MATCHER_MATCHES(emptyArr, eachElement(3)); });

    test("eachElement(3) matches array where all elements are 3",
         [&] { EXPECT_MATCHER_MATCHES(all3Arr, eachElement(3)); });

    test("eachElement(3) does not match array where an element is not 3",
         [&] { EXPECT_MATCHER_FAILS(arr, eachElement(3)); });

    test("eachElement works with element sub-matchers", [&] {
        EXPECT_MATCHER_MATCHES(arr, eachElement(isOdd));
        EXPECT_MATCHER_MATCHES(arr, eachElement(isPositive));
        EXPECT_MATCHER_MATCHES(arr, eachElement(isLessThan(100)));
        EXPECT_MATCHER_FAILS(arr, eachElement(isLessThan(5)));
    });

    test("eachElement can be chained with itself", [&] {
        EXPECT_MATCHER_MATCHES(matrix, eachElement(eachElement(isLessThan(5))));
    });

    test("anyElement(3) does not match empty array",
         [&] { EXPECT_MATCHER_FAILS(emptyArr, anyElement(3)); });

    test("anyElement(3) matches array where all elements are 3",
         [&] { EXPECT_MATCHER_MATCHES(all3Arr, anyElement(3)); });

    test("anyElement(3) matches array where at least one element is 3",
         [&] { EXPECT_MATCHER_MATCHES(arr, anyElement(3)); });

    test("anyElement(3) does not match array where no element is 3", [&] {
        EXPECT_MATCHER_FAILS(vector<int>{1, 2, 4, 5, 6}, anyElement(3));
    });

    test("anyElement works with element sub-matchers", [&] {
        EXPECT_MATCHER_MATCHES(arr, anyElement(isOdd));
        EXPECT_MATCHER_MATCHES(arr, anyElement(isPositive));
        EXPECT_MATCHER_MATCHES(arr, anyElement(isLessThan(3)));
        EXPECT_MATCHER_FAILS(arr, anyElement(isGreaterThan(100)));
    });

    test("anyElement can be chained with itself", [&] {
        EXPECT_MATCHER_MATCHES(matrix, anyElement(anyElement(isLessThan(2))));
    });

    test("anyElement and eachElement can be chained together", [&] {
        EXPECT_MATCHER_MATCHES(matrix, anyElement(eachElement(isEven)));
        EXPECT_MATCHER_FAILS(matrix, anyElement(eachElement(isOdd)));

        EXPECT_MATCHER_MATCHES(matrix, eachElement(anyElement(isEven)));
        EXPECT_MATCHER_FAILS(matrix, eachElement(anyElement(isOdd)));
    });

    test(
      "anyElement and eachElement can be chained with hasSize matchers", [&] {
          EXPECT_MATCHER_MATCHES(matrix, anyElement(hasSize(isGreaterThan(3))));
          EXPECT_MATCHER_FAILS(matrix, anyElement(hasSize(isLessThan(2))));
          EXPECT_MATCHER_MATCHES(matrix, eachElement(hasSize(isLessThan(100))));
          EXPECT_MATCHER_FAILS(matrix, eachElement(hasSize(isGreaterThan(2))));
      });
}
