#pragma once

#include <kktest_ext/matchers/matcher.hpp>

namespace kktest::matchers::detail {

class IsNullptrMatcher: public StatelessMatcher {
 public:
    bool matches(void*const& obj) const {
        return obj == nullptr;
    }

    void describe(Description* description) const {
        (*description) << "nullptr";
    }

    void describeFailure(Description* description) const {
        (*description) << "non-null pointer";
    }
};

class IsNotNullptrMatcher: public StatelessMatcher {
 public:
    bool matches(void*const& obj) const {
        return obj != nullptr;
    }

    void describe(Description* description) const {
        (*description) << "non-null pointer";
    }

    void describeFailure(Description* description) const {
        (*description) << "nullptr";
    }
};

}
