#ifndef RUNTIME_TESTING_MATCHER_POINTER_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_POINTER_MATCHERS_H_

#include "matcher.hpp"

namespace matcher {

class IsNullptrMatcher: public Matcher {
public:
    bool matches(void*const& obj);

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

class IsNotNullptrMatcher: public Matcher {
public:
    bool matches(void*const& obj);

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

}

#endif
