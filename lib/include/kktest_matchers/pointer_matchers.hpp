#ifndef KKTEST_INCLUDE_KKTEST_MATCHERS_POINTER_MATCHERS_H_
#define KKTEST_INCLUDE_KKTEST_MATCHERS_POINTER_MATCHERS_H_

#include "../kktest_matcher.hpp"


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
