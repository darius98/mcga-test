#pragma once

#include <kktest_ext/matchers/matcher.hpp>
#include <kktest_ext/matchers/detail/decl.hpp>

namespace kktest::matchers {

namespace detail {

class IsTrueMatcher: public StatelessMatcher {
 public:
    constexpr IsTrueMatcher() = default;

    bool matches(const bool& object) const {
        return object;
    }

    void describe(Description* description) const override {
        (*description) << "true";
    }

    void describeFailure(Description* description) const override {
        (*description) << "false";
    }
};

class IsFalseMatcher: public StatelessMatcher {
 public:
    constexpr IsFalseMatcher() = default;

    bool matches(const bool& object) const {
        return !object;
    }

    void describe(Description* description) const override {
        (*description) << "false";
    }

    void describeFailure(Description* description) const override {
        (*description) << "true";
    }
};

}

constexpr detail::IsTrueMatcher isTrue;

constexpr detail::IsFalseMatcher isFalse;

}
