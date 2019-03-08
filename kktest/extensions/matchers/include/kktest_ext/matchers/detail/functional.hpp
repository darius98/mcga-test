#pragma once

#include <functional>

#include <kktest_ext/matchers/matcher.hpp>

namespace kktest::matchers::detail {

class ThrowsAnythingMatcher: public StatelessMatcher {
 public:
    bool matches(const Executable& func) const {
        try {
            func();
            return false;
        } catch(...) {
            return true;
        }
    }

    void describe(Description* description) const override {
        (*description) << "a function that throws";
    }

    void describeFailure(Description* description) const override {
        (*description) << "a function that did not throw";
    }
};

enum ThrowsSpecificState {
    THROWS_CORRECTLY,
    DOESNT_THROW,
    THROWS_DIFFERENT,
};

template<class E>
class ThrowsSpecificMatcher: public StatefulMatcher<ThrowsSpecificState> {
 public:
    constexpr ThrowsSpecificMatcher() = default;

    bool matches(const Executable& func, ThrowsSpecificState* state) const {
        try {
            func();
            *state = DOESNT_THROW;
            return false;
        } catch(const E& exception) {
            *state = THROWS_CORRECTLY;
            return true;
        } catch(...) {
            *state = THROWS_DIFFERENT;
            return false;
        }
    }

    void describe(Description* description) const override {
        (*description) << "a function that throws " << typeid(E).name();
    }

    void describeFailure(Description* description,
                         ThrowsSpecificState* state) const override {
        if (*state == DOESNT_THROW) {
            (*description) << "a function that did not throw";
        }
        if (*state == THROWS_DIFFERENT) {
            (*description) << "a function that throws something else";
        }
    }
};

}