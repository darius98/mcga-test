#include "matchers/matcher_test.hpp"

int main() {
    truthMatcherTest();
    comparisonMatcherTest();
    iterableMatcherTest();
    return numFailedTests;
}
