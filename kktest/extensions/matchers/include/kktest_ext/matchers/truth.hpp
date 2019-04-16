#pragma once

#include <kktest_ext/matchers/detail/truth.hpp>

namespace mcga::test::matchers {

constexpr detail::IsTrueMatcher isTrue;

constexpr detail::IsFalseMatcher isFalse;

}  // namespace mcga::test::matchers
