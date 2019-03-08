#pragma once

#include <kktest_ext/matchers/detail/numeric.hpp>

namespace kktest::matchers {

constexpr detail::IsPositiveMatcher isPositive;

constexpr detail::IsNegativeMatcher isNegative;

constexpr detail::IsEvenMatcher isEven;

constexpr detail::IsOddMatcher isOdd;

constexpr detail::IsZeroMatcher isZero;

template<class T> auto isAlmostEqualTo(const T& target, const double& eps) {
    return detail::IsAlmostEqualMatcher(target, eps);
}

}
