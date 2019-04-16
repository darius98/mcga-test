#pragma once

#include <kktest_ext/matchers/detail/functional.hpp>

namespace kktest::matchers {

template<class E>
constexpr detail::ThrowsSpecificMatcher<E> throwsA() {
    return detail::ThrowsSpecificMatcher<E>();
}

constexpr detail::ThrowsAnythingMatcher throws;

}  // namespace kktest::matchers
