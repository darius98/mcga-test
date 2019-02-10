#include <kktest_ext/core_matchers_impl/numeric_matchers.hpp>

namespace kktest {
namespace core_matchers {

void IsPositiveMatcher::describe(Description* description) {
    (*description) << "a positive number";
}

IsPositiveMatcher isPositive;

void IsNegativeMatcher::describe(Description* description) {
    (*description) << "a negative number";
}

IsNegativeMatcher isNegative;

void IsEvenMatcher::describe(Description* description) {
    (*description) << "an even number";
}

IsEvenMatcher isEven;

void IsOddMatcher::describe(Description* description) {
    (*description) << "an odd number";
}

IsOddMatcher isOdd;

void IsZeroMatcher::describe(Description* description) {
    (*description) << "zero";
}

IsZeroMatcher isZero;

}
}
