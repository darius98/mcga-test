#pragma once

#include <string>

#include <kktest_ext/matchers/matcher.hpp>

namespace kktest::matchers::detail {

class CharInStringMatcher: public StatelessMatcher {
 public:
    constexpr explicit CharInStringMatcher(const char* container,
                                           const char* expectation = nullptr):
            container(container), expectation(expectation) {}

    bool matches(const char& ch) const {
        return strchr(container, ch) != nullptr;
    }

    void describe(Description* description) const override {
        if (expectation != nullptr) {
            (*description) << expectation;
        } else {
            (*description) << "character in '" << container << "'";
        }
    }

    void describeFailure(Description* description) const override {
        if (expectation != nullptr) {
            (*description) << "not " << expectation;
        } else {
            (*description) << "character that is not in '" << container << "'";
        }
    }

 private:
    const char* container;
    const char* expectation;
};

class IsSubstringMatcher: public StatelessMatcher {
 public:
    explicit IsSubstringMatcher(std::string container):
            container(std::move(container)) {}

    bool matches(const std::string& object) const {
        return container.find(object) != std::string::npos;
    }

    void describe(Description* description) const override {
        (*description) << "a substring of '" << container << "'";
    }

    void describeFailure(Description* description) const override {
        (*description) << "not a substring";
    }

 private:
    std::string container;
};

}
