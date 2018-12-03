#include <kktest.hpp>
#include <kktest_ext/core_matchers_impl/iterable_matchers.hpp>
#include <kktest_ext/core_matchers_impl/numeric_matchers.hpp>

using kktest::expect;
using kktest::test;
using kktest::core_matchers::anyElement;
using kktest::core_matchers::eachElement;
using kktest::core_matchers::hasSize;
using kktest::core_matchers::isEven;
using kktest::core_matchers::isEmpty;
using kktest::core_matchers::isGreaterThan;
using kktest::core_matchers::isLessThan;
using kktest::core_matchers::isNotEmpty;
using kktest::core_matchers::isPositive;
using kktest::core_matchers::isOdd;
using std::vector;

void kkTestCase(IterableMatchers) {
    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15};
    vector<vector<int>> matrix = {{1, 2, 3, 4}, {2, 3, 4}, {2, 3}, {2, 4}};
    vector<int> emptyArr = {};
    vector<int> all3Arr = {3, 3, 3, 3, 3};

    test("isEmpty matches empty array", [&] {
        expect(isEmpty.matches(emptyArr));
    });

    test("isEmpty does not match non-empty arrays", [&] {
        expect(!isEmpty.matches(arr));
        expect(!isEmpty.matches(matrix));
    });

    test("isNotEmpty does not match empty array", [&] {
        expect(!isNotEmpty.matches(emptyArr));
    });

    test("isNotEmpty matches non-empty arrays", [&] {
        expect(isNotEmpty.matches(arr));
        expect(isNotEmpty.matches(matrix));
    });

    test("hasSize(8) matches iterable with size 8", [&] {
        expect(hasSize(8).matches(arr));
    });

    test("hasSize(5) does not match iterable with size 8", [&] {
        expect(!hasSize(5).matches(arr));
    });

    test("hasSize works with size sub-matcher", [&] {
        expect(hasSize(isLessThan(10)).matches(arr));
        expect(hasSize(isGreaterThan(7)).matches(arr));
        expect(hasSize(isEven).matches(arr));
        expect(!hasSize(isOdd).matches(arr));
        expect(!hasSize(isGreaterThan(20)).matches(arr));
    });

    test("eachElement(3) matches empty array", [&] {
        expect(eachElement(3).matches(emptyArr));
    });

    test("eachElement(3) matches array where all elements are 3", [&] {
        expect(eachElement(3).matches(all3Arr));
    });

    test("eachElement(3) does not match array where an element is not 3", [&] {
        expect(!eachElement(3).matches(arr));
    });

    test("eachElement works with element sub-matchers", [&] {
        expect(eachElement(isOdd).matches(arr));
        expect(eachElement(isPositive).matches(arr));
        expect(eachElement(isLessThan(100)).matches(arr));
        expect(!eachElement(isLessThan(5)).matches(arr));
    });

    test("eachElement can be chained with itself", [&] {
        expect(eachElement(eachElement(isLessThan(5))).matches(matrix));
    });

    test("anyElement(3) does not match empty array", [&] {
        expect(!anyElement(3).matches(emptyArr));
    });

    test("anyElement(3) matches array where all elements are 3", [&] {
        expect(anyElement(3).matches(all3Arr));
    });

    test("anyElement(3) matches array where at least one element is 3", [&] {
        expect(anyElement(3).matches(arr));
    });

    test("anyElement(3) does not match array where no element is 3", [&] {
        expect(!anyElement(3).matches(vector<int>{1, 2, 4, 5, 6}));
    });

    test("anyElement works with element sub-matchers", [&] {
        expect(anyElement(isOdd).matches(arr));
        expect(anyElement(isPositive).matches(arr));
        expect(anyElement(isLessThan(3)).matches(arr));
        expect(!anyElement(isGreaterThan(100)).matches(arr));
    });

    test("anyElement can be chained with itself", [&] {
        expect(anyElement(anyElement(isLessThan(2))).matches(matrix));
    });

    test("anyElement and eachElement can be chained together", [&] {
        expect(anyElement(eachElement(isEven)).matches(matrix));
        expect(!anyElement(eachElement(isOdd)).matches(matrix));

        expect(eachElement(anyElement(isEven)).matches(matrix));
        expect(!eachElement(anyElement(isOdd)).matches(matrix));
    });

    test("anyElement and eachElement can be chained with hasSize matchers", [&] {
        expect(anyElement(hasSize(isGreaterThan(3))).matches(matrix));
        expect(!anyElement(hasSize(isLessThan(2))).matches(matrix));
        expect(eachElement(hasSize(isLessThan(100))).matches(matrix));
        expect(!eachElement(hasSize(isGreaterThan(2))).matches(matrix));
    });
}
