#ifndef RUNTIME_TESTING_MATCHER_POINTER_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_POINTER_MATCHERS_H_

#include "matcher.hpp"

namespace matcher {

class IsNullptrMatcher: public Matcher<void*> {
public:
    bool matches(void*const& obj) override;

    void describeExpectation(Description* description) override;

    void describeSuccess(Description* description) override;

    void describeFailure(Description* description) override;
};

class IsNotNullptrMatcher: public Matcher<void*> {
public:
    bool matches(void*const& obj) override;

    void describeExpectation(Description* description) override;

    void describeSuccess(Description* description) override;

    void describeFailure(Description* description) override;
};

}

#endif
