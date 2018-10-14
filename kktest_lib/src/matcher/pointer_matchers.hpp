#ifndef KKTEST_MATCHER_POINTER_MATCHERS_H_
#define KKTEST_MATCHER_POINTER_MATCHERS_H_

#include "matcher.hpp"


namespace kktest {

class IsNullptrMatcher: public Matcher {
public:
    bool matches(void*const& obj);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsNullptrMatcher* isNull;

class IsNotNullptrMatcher: public Matcher {
public:
    bool matches(void*const& obj);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsNotNullptrMatcher* isNotNull;

}

#endif
