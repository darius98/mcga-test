#pragma once

#include <kktest_ext/matchers/matcher.hpp>
#include <kktest_ext/matchers/detail/decl.hpp>

namespace kktest::matchers {

namespace detail {

class IsNullptrMatcher: public StatelessMatcher {
 public:
    bool matches(void*const& obj) const {
        return obj == nullptr;
    }

    void describe(Description* description) const override {
        (*description) << "nullptr";
    }

    void describeFailure(Description* description) const override {
        (*description) << "non-null pointer";
    }
};

class IsNotNullptrMatcher: public StatelessMatcher {
 public:
    bool matches(void*const& obj) const {
        return obj != nullptr;
    }

    void describe(Description* description) const override {
        (*description) << "non-null pointer";
    }

    void describeFailure(Description* description) const override {
        (*description) << "nullptr";
    }
};

}

constexpr detail::IsNullptrMatcher isNull;

constexpr detail::IsNotNullptrMatcher isNotNull;

}
