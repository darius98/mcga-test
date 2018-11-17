#include <kktest>
#include <kktest_matchers>

using namespace kktest;
using namespace std;

void kkTestCase() {
    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15};
    vector<vector<int>> matrix = {{1, 2, 3, 4}, {2, 3, 4}, {2, 3}, {2, 4}};
    vector<int> emptyArr = {};
    vector<int> all3Arr = {3, 3, 3, 3, 3};

    kkTest("isEmpty matches empty array", [&]() {
        kkExpect(isEmpty.matches(emptyArr));
    });

    kkTest("isEmpty does not match non-empty arrays", [&]() {
        kkExpect(!isEmpty.matches(arr));
        kkExpect(!isEmpty.matches(matrix));
    });

    kkTest("isNotEmpty does not match empty array", [&]() {
        kkExpect(!isNotEmpty.matches(emptyArr));
    });

    kkTest("isNotEmpty matches non-empty arrays", [&]() {
        kkExpect(isNotEmpty.matches(arr));
        kkExpect(isNotEmpty.matches(matrix));
    });

    kkTest("hasSize(8) matches iterable with size 8", [&]() {
        kkExpect(hasSize(8).matches(arr));
    });

    kkTest("hasSize(5) does not match iterable with size 8", [&]() {
        kkExpect(!hasSize(5).matches(arr));
    });

    kkTest("hasSize works with size sub-matcher", [&]() {
        kkExpect(hasSize(isLessThan(10)).matches(arr));
        kkExpect(hasSize(isGreaterThan(7)).matches(arr));
        kkExpect(hasSize(isEven).matches(arr));
        kkExpect(!hasSize(isOdd).matches(arr));
        kkExpect(!hasSize(isGreaterThan(20)).matches(arr));
    });

    kkTest("eachElement(3) matches empty array", [&]() {
        kkExpect(eachElement(3).matches(emptyArr));
    });

    kkTest("eachElement(3) matches array where all elements are 3", [&]() {
        kkExpect(eachElement(3).matches(all3Arr));
    });

    kkTest("eachElement(3) does not match array where an element is not 3", [&]() {
        kkExpect(!eachElement(3).matches(arr));
    });

    kkTest("eachElement works with element sub-matchers", [&]() {
        kkExpect(eachElement(isOdd).matches(arr));
        kkExpect(eachElement(isPositive).matches(arr));
        kkExpect(eachElement(isLessThan(100)).matches(arr));
        kkExpect(!eachElement(isLessThan(5)).matches(arr));
    });

    kkTest("eachElement can be chained with itself", [&]() {
        kkExpect(eachElement(eachElement(isLessThan(5))).matches(matrix));
    });

    kkTest("anyElement(3) does not match empty array", [&]() {
        kkExpect(!anyElement(3).matches(emptyArr));
    });

    kkTest("anyElement(3) matches array where all elements are 3", [&]() {
        kkExpect(anyElement(3).matches(all3Arr));
    });

    kkTest("anyElement(3) matches array where at least one element is 3", [&]() {
        kkExpect(anyElement(3).matches(arr));
    });

    kkTest("anyElement(3) does not match array where no element is 3", [&]() {
        kkExpect(!anyElement(3).matches(vector<int>{1, 2, 4, 5, 6}));
    });

    kkTest("anyElement works with element sub-matchers", [&]() {
        kkExpect(anyElement(isOdd).matches(arr));
        kkExpect(anyElement(isPositive).matches(arr));
        kkExpect(anyElement(isLessThan(3)).matches(arr));
        kkExpect(!anyElement(isGreaterThan(100)).matches(arr));
    });

    kkTest("anyElement can be chained with itself", [&]() {
        kkExpect(anyElement(anyElement(isLessThan(2))).matches(matrix));
    });

    kkTest("anyElement and eachElement can be chained together", [&]() {
        kkExpect(anyElement(eachElement(isEven)).matches(matrix));
        kkExpect(!anyElement(eachElement(isOdd)).matches(matrix));

        kkExpect(eachElement(anyElement(isEven)).matches(matrix));
        kkExpect(!eachElement(anyElement(isOdd)).matches(matrix));
    });

    kkTest("anyElement and eachElement can be chained with hasSize matchers", [&]() {
        kkExpect(anyElement(hasSize(isGreaterThan(3))).matches(matrix));
        kkExpect(!anyElement(hasSize(isLessThan(2))).matches(matrix));
        kkExpect(eachElement(hasSize(isLessThan(100))).matches(matrix));
        kkExpect(!eachElement(hasSize(isGreaterThan(2))).matches(matrix));
    });

}