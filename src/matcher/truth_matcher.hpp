#ifndef RUNTIME_TESTING_MATCHER_TRUTH_MATCHER_H_
#define RUNTIME_TESTING_MATCHER_TRUTH_MATCHER_H_

#include "matcher.hpp"


namespace matcher {

class IsTrueMatcher: public Matcher<bool> {
public:
    bool matches(const bool& object) override;

    void describe(const bool& object, Description* description) override;
};

class IsFalseMatcher: public Matcher<bool> {
public:
    bool matches(const bool& object) override;

    void describe(const bool& object, Description* description) override;
};

} // namespace matcher

#endif
