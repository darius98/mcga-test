#pragma once

#include <kktest_ext/matchers/detail/composite.hpp>

namespace mcga::test::matchers {

template<class A, class B>
constexpr auto both(const A& a, const B& b) {
    if constexpr (std::is_base_of_v<Matcher, A>) {
        if constexpr (std::is_base_of_v<Matcher, B>) {
            return detail::AndMatcher(a, b);
        } else {
            return detail::AndMatcher(a, isEqualTo(b));
        }
    } else {
        if constexpr (std::is_base_of_v<Matcher, B>) {
            return detail::AndMatcher(isEqualTo(a), b);
        } else {
            return detail::AndMatcher(isEqualTo(a), isEqualTo(b));
        }
    }
}

template<class A, class B>
constexpr auto either(const A& a, const B& b) {
    if constexpr (std::is_base_of_v<Matcher, A>) {
        if constexpr (std::is_base_of_v<Matcher, B>) {
            return detail::OrMatcher(a, b);
        } else {
            return detail::OrMatcher(a, isEqualTo(b));
        }
    } else {
        if constexpr (std::is_base_of_v<Matcher, B>) {
            return detail::OrMatcher(isEqualTo(a), b);
        } else {
            return detail::OrMatcher(isEqualTo(a), isEqualTo(b));
        }
    }
}

template<class T>
constexpr auto isNot(const T& x) {
    if constexpr (std::is_base_of_v<Matcher, T>) {
        return detail::NotMatcher<T>(x);
    } else {
        return detail::NotMatcher(isEqualTo(x));
    }
}

}  // namespace mcga::test::matchers
