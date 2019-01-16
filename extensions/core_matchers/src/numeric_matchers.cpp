#include <kktest_ext/core_matchers_impl/numeric_matchers.hpp>

namespace kktest {
namespace core_matchers {

void IsPositiveMatcher::describe(Description* description) {
    (*description) << "a positive number";
}

void IsPositiveMatcher::describeMismatch(Description* description) {
    (*description) << "a non-positive number";
}

IsPositiveMatcher isPositive;

void IsNegativeMatcher::describe(Description* description) {
    (*description) << "a negative number";
}

void IsNegativeMatcher::describeMismatch(Description* description) {
    (*description) << "a non-negative number";
}

IsNegativeMatcher isNegative;

void IsEvenMatcher::describe(Description* description) {
    (*description) << "an even number";
}

void IsEvenMatcher::describeMismatch(Description* description) {
    (*description) << "an odd number";
}

IsEvenMatcher isEven;

void IsOddMatcher::describe(Description* description) {
    (*description) << "an odd number";
}

void IsOddMatcher::describeMismatch(Description* description) {
    (*description) << "an even number";
}

IsOddMatcher isOdd;

void IsZeroMatcher::describe(Description* description) {
    (*description) << "zero";
}

IsZeroMatcher isZero;

}
}
