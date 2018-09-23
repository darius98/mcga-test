#include "../../src/matcher/core_matchers.hpp"

#include "matcher_test.hpp"

using namespace std;
using namespace matcher;

int main() {
    int failed = 0;
    cout << "isTrue called on 'true': ";
    failed += !expectMatch(true, isTrue);
    cout << "isTrue called on 'false': ";
    failed += !expectMatch(false, isTrue, false, "true, got false");
    cout << "isFalse called on 'true': ";
    failed += !expectMatch(true, isFalse, false, "false, got true");
    cout << "isFalse called on 'false': ";
    failed += !expectMatch(false, isFalse);
    return failed;
}
