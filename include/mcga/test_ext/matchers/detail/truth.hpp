#pragma once

#include "mcga/test_ext/matchers/matcher.hpp"

namespace mcga::test::matchers::detail {

struct IsTrueMatcher : StatelessMatcher {
    bool matches(const bool& object) const {
        return object;
    }

    void describe(Description* description) const {
        (*description) << "true";
    }

    void describeFailure(Description* description) const {
        (*description) << "false";
    }
};

struct IsFalseMatcher : StatelessMatcher {
    bool matches(const bool& object) const {
        return !object;
    }

    void describe(Description* description) const {
        (*description) << "false";
    }

    void describeFailure(Description* description) const {
        (*description) << "true";
    }
};

}  // namespace mcga::test::matchers::detail
