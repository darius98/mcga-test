#ifndef KTEST_MATCHER_TRUTH_MATCHER_H_
#define KTEST_MATCHER_TRUTH_MATCHER_H_

#include "matcher.hpp"


namespace ktest {

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

} // namespace ktest

#endif
