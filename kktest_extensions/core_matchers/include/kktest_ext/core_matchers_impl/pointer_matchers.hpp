#ifndef KKTEST_EXT_CORE_MATCHERS_IMPL_POINTER_MATCHERS_H_
#define KKTEST_EXT_CORE_MATCHERS_IMPL_POINTER_MATCHERS_H_

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {

class IsNullptrMatcher: public Matcher {
public:
    bool matches(void*const& obj);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsNullptrMatcher isNull;

class IsNotNullptrMatcher: public Matcher {
public:
    bool matches(void*const& obj);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsNotNullptrMatcher isNotNull;

}

#endif
