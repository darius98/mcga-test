#pragma once

#include <kktest_ext/matchers/matcher.hpp>
#include <kktest_ext/matchers/detail/decl.hpp>

namespace kktest::matchers {

namespace detail {

class IsTrueMatcher: public Matcher {
 public:
    bool matches(const bool& object) {
        return object;
    }

    void describe(Description* description) {
        (*description) << "true";
    }

    void describeMismatch(Description* description, const bool&) {
        (*description) << "false";
    }
};

class IsFalseMatcher: public Matcher {
 public:
    bool matches(const bool& object) {
        return !object;
    }

    void describe(Description* description) {
        (*description) << "false";
    }

    void describeMismatch(Description* description, const bool&) {
        (*description) << "true";
    }
};

}

static detail::IsTrueMatcher isTrue;

static detail::IsFalseMatcher isFalse;

}
