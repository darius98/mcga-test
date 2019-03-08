#pragma once

#include <kktest_ext/matchers/detail/comparison.hpp>

namespace kktest::matchers {

template<class T> auto isEqualTo(const T& object) {
    return detail::EqualityMatcher(object);
}

template<class T> auto isNotEqualTo(const T& object) {
    return detail::NonEqualityMatcher(object);
}

template<class T> auto isLessThan(const T& object) {
    return detail::IsLessThanMatcher(object);
}

template<class T> auto isLessThanEqual(const T& object) {
    return detail::IsLessThanEqualMatcher(object);
}

template<class T> auto isGreaterThan(const T& object) {
    return detail::IsGreaterThanMatcher(object);
}

template<class T> auto isGreaterThanEqual(const T& object) {
    return detail::IsGreaterThanEqualMatcher(object);
}

template<class T> auto isIdenticalTo(const T& object) {
    return detail::IdentityMatcher(object);
}

}
