#include "extensions/matchers/include/kktest_ext/matchers_impl/numeric_matchers.hpp"

#include "core/src/export.hpp"

namespace kktest {
namespace matchers {

KKTEST_EXPORT detail::IsPositiveMatcher isPositive;

KKTEST_EXPORT detail::IsNegativeMatcher isNegative;

KKTEST_EXPORT detail::IsEvenMatcher isEven;

KKTEST_EXPORT detail::IsOddMatcher isOdd;

KKTEST_EXPORT detail::IsZeroMatcher isZero;

namespace detail {

KKTEST_EXPORT void IsPositiveMatcher::describe(Description* description) {
    (*description) << "a positive number";
}

KKTEST_EXPORT void IsNegativeMatcher::describe(Description* description) {
    (*description) << "a negative number";
}

KKTEST_EXPORT void IsEvenMatcher::describe(Description* description) {
    (*description) << "an even number";
}

KKTEST_EXPORT void IsOddMatcher::describe(Description* description) {
    (*description) << "an odd number";
}

KKTEST_EXPORT void IsZeroMatcher::describe(Description* description) {
    (*description) << "zero";
}

}
}
}
