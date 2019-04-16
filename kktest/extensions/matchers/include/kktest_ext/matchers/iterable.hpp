#pragma once

#include <kktest_ext/matchers/detail/iterable.hpp>

namespace mcga::test::matchers {

template<class T>
constexpr auto hasSize(const T& size) {
    if constexpr (std::is_base_of_v<Matcher, T>) {
        return detail::IterableSizeMatcher<T>(size);
    } else {
        return detail::IterableSizeMatcher(isEqualTo(size));
    }
}

template<class T>
constexpr auto eachElement(const T& each) {
    if constexpr (std::is_base_of_v<Matcher, T>) {
        return detail::IterableEachMatcher<T>(each);
    } else {
        return detail::IterableEachMatcher(isEqualTo(each));
    }
}

template<class T>
constexpr auto anyElement(const T& any) {
    if constexpr (std::is_base_of_v<Matcher, T>) {
        return detail::IterableAnyMatcher<T>(any);
    } else {
        return detail::IterableAnyMatcher(isEqualTo(any));
    }
}

constexpr detail::IsEmptyMatcher isEmpty;

constexpr detail::IsNotEmptyMatcher isNotEmpty;

}  // namespace mcga::test::matchers
