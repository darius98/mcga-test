#include <map>
#include <vector>


#include "../../src/matcher/core_matchers.hpp"

#include "matcher_test.hpp"

using namespace std;
using namespace matcher;

void iterableMatcherTest() {
    auto vec = vector<int>{};
    test("isEmpty called on empty vector", vec, isEmpty);
    test("isNotEmpty called on empty vector", vec, isNotEmpty, false,
         "non-empty container, got '[]'");
    vec.push_back(3);
    test("isEmpty called on non-empty vector", vec, isEmpty,
            false, "empty container, got '[3]'");
    test("isNotEmpty called on non-empty vector", vec, isNotEmpty);
    vec.push_back(5);
    test("isEmpty called on non-empty vector (2)", vec, isEmpty,
         false, "empty container, got '[3, 5]'");
    test("isNotEmpty called on non-empty vector (2)", vec, isNotEmpty);

    auto s = set<int>{};
    test("isEmpty called on empty set", s, isEmpty);
    test("isNotEmpty called on empty set", s, isNotEmpty, false,
         "non-empty container, got '{}'");
    s.insert(3);
    test("isEmpty called on non-empty set", s, isEmpty,
         false, "empty container, got '{3}'");
    test("isNotEmpty called on non-empty set", s, isNotEmpty);
    s.insert(5);
    test("isEmpty called on non-empty set (2)", s, isEmpty,
         false, "empty container, got '{3, 5}'");
    test("isNotEmpty called on non-empty set (2)", s, isNotEmpty);

    vec = vector<int>{1, 2, 3}; // size 3
    test("hasSize(3) called on vector w size 3", vec, hasSize(3));
    test("hasSize(greaterThan(2)) called on vector w size 3", vec,
         hasSize(greaterThan(2)));
    test("hasSize(greaterThan(4)) called on vector w size 3", vec,
         hasSize(greaterThan(4)),
         false, "container of size greater than '4', got '3': '[1, 2, 3]'");
}
