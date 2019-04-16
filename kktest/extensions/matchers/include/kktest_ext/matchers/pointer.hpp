#pragma once

#include <kktest_ext/matchers/detail/pointer.hpp>

namespace kktest::matchers {

constexpr detail::IsNullptrMatcher isNull;

constexpr detail::IsNotNullptrMatcher isNotNull;

}  // namespace kktest::matchers
