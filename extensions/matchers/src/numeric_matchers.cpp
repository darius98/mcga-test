#include "extensions/matchers/include/kktest_ext/matchers_impl/numeric_matchers.hpp"

namespace kktest {
namespace matchers {

detail::IsPositiveMatcher isPositive;

detail::IsNegativeMatcher isNegative;

detail::IsEvenMatcher isEven;

detail::IsOddMatcher isOdd;

detail::IsZeroMatcher isZero;

namespace detail {

void IsPositiveMatcher::describe(Description* description) {
    (*description) << "a positive number";
}

void IsNegativeMatcher::describe(Description* description) {
    (*description) << "a negative number";
}

void IsEvenMatcher::describe(Description* description) {
    (*description) << "an even number";
}

void IsOddMatcher::describe(Description* description) {
    (*description) << "an odd number";
}

void IsZeroMatcher::describe(Description* description) {
    (*description) << "zero";
}

}
}
}
