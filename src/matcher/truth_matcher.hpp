#ifndef KKTEST_MATCHER_TRUTH_MATCHER_H_
#define KKTEST_MATCHER_TRUTH_MATCHER_H_

#include "matcher/matcher.hpp"


namespace kktest {

class IsTrueMatcher: public Matcher {
public:
    bool matches(const bool& object);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

class IsFalseMatcher: public Matcher {
public:
    bool matches(const bool& object);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

} // namespace kktest

#endif
