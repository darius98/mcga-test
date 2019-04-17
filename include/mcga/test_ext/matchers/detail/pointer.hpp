#pragma once

#include "mcga/test_ext/matchers/matcher.hpp"

namespace mcga::test::matchers::detail {

struct IsNullptrMatcher : StatelessMatcher {
    bool matches(void* const& obj) const {
        return obj == nullptr;
    }

    void describe(Description* description) const {
        (*description) << "nullptr";
    }

    void describeFailure(Description* description) const {
        (*description) << "non-null pointer";
    }
};

struct IsNotNullptrMatcher : StatelessMatcher {
    bool matches(void* const& obj) const {
        return obj != nullptr;
    }

    void describe(Description* description) const {
        (*description) << "non-null pointer";
    }

    void describeFailure(Description* description) const {
        (*description) << "nullptr";
    }
};

}  // namespace mcga::test::matchers::detail
