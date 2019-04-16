#pragma once

#include <kktest_ext/matchers/matcher.hpp>

namespace kktest::matchers::detail {

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

}  // namespace kktest::matchers::detail
