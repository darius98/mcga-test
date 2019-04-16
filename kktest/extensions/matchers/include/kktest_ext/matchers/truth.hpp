#pragma once

#include <kktest_ext/matchers/detail/truth.hpp>

namespace kktest::matchers {

constexpr detail::IsTrueMatcher isTrue;

constexpr detail::IsFalseMatcher isFalse;

}  // namespace kktest::matchers
