#ifndef KTEST_MATCHER_POINTER_MATCHERS_H_
#define KTEST_MATCHER_POINTER_MATCHERS_H_

#include "matcher.hpp"

namespace ktest {

class IsNullptrMatcher: public Matcher {
public:
    bool matches(void*const& obj);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

class IsNotNullptrMatcher: public Matcher {
public:
    bool matches(void*const& obj);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

}

#endif
