#pragma once

#include <kktest_ext/matchers/detail/comparison.hpp>

namespace kktest::matchers {

template<class T>
constexpr auto isEqualTo(const T& object) {
    return detail::EqualityMatcher<T>(object);
}

template<class T>
constexpr auto isNotEqualTo(const T& object) {
    return detail::NonEqualityMatcher<T>(object);
}

template<class T>
constexpr auto isLessThan(const T& object) {
    return detail::IsLessThanMatcher<T>(object);
}

template<class T>
constexpr auto isLessThanEqual(const T& object) {
    return detail::IsLessThanEqualMatcher<T>(object);
}

template<class T>
constexpr auto isGreaterThan(const T& object) {
    return detail::IsGreaterThanMatcher<T>(object);
}

template<class T>
constexpr auto isGreaterThanEqual(const T& object) {
    return detail::IsGreaterThanEqualMatcher<T>(object);
}

template<class T>
constexpr auto isIdenticalTo(const T& object) {
    return detail::IdentityMatcher<T>(object);
}

}  // namespace kktest::matchers
