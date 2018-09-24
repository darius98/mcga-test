#include <map>
#include <vector>


#include "../../src/matcher/core_matchers.hpp"

#include "matcher_test.hpp"

using namespace std;
using namespace matcher;

int main() {
    auto vec = vector<int>{};
    test("isEmpty called on empty vector", vec, isEmpty<vector<int>>());
    vec.push_back(3);
    test("isEmpty called on non-empty vector", vec, isEmpty<vector<int>>(),
            false, "empty container, got '[3]'");
    vec.push_back(5);
    test("isEmpty called on non-empty vector (2)", vec, isEmpty<vector<int>>(),
         false, "empty container, got '[3, 5]'");

    auto s = set<int>{};
    test("isEmpty called on empty set", s, isEmpty<set<int>>());
    s.insert(3);
    test("isEmpty called on non-empty set", s, isEmpty<set<int>>(),
         false, "empty container, got '{3}'");
    s.insert(5);
    test("isEmpty called on non-empty set (2)", s, isEmpty<set<int>>(),
         false, "empty container, got '{3, 5}'");

    vec = vector<int>{1, 2, 3}; // size 3
    test("hasSize(3) called on vector w size 3", vec, hasSize<vector<int>>(3));
    test("hasSize(greaterThan(2)) called on vector w size 3", vec,
            hasSize<vector<int>>(greaterThan<vector<int>::size_type>(2)));
    test("hasSize(greaterThan(4)) called on vector w size 3", vec,
         hasSize<vector<int>>(greaterThan<vector<int>::size_type>(4)),
         false, "container of size greater than '4', got '3': '[1, 2, 3]'");

    return numFailedTests;
}
