#include "../../src/matcher/core_matchers.hpp"

#include "matcher_test.hpp"

using namespace std;
using namespace matcher;

void truthMatcherTest() {
    test("isTrue called on 'true'", true, isTrue);
    test("isTrue called on 'false'", false, isTrue, false, "true, got false");
    test("isFalse called on 'true'", true, isFalse, false, "false, got true");
    test("isFalse called on 'false'", false, isFalse);
}
