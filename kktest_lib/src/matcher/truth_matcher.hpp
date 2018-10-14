#ifndef KKTEST_MATCHER_TRUTH_MATCHER_H_
#define KKTEST_MATCHER_TRUTH_MATCHER_H_

#include "matcher.hpp"


namespace kktest {

class IsTrueMatcher: public Matcher {
public:
    bool matches(const bool& object);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsTrueMatcher* isTrue;

class IsFalseMatcher: public Matcher {
public:
    bool matches(const bool& object);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsFalseMatcher* isFalse;

} // namespace kktest

#endif