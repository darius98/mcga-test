#ifndef KTEST_MATCHER_NUMERIC_MATCHERS_H_
#define KTEST_MATCHER_NUMERIC_MATCHERS_H_

#include <cmath>
#include "matcher.hpp"


namespace ktest {

class IsPositiveMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object > 0;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

class IsNegativeMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object < 0;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

class IsEvenMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 0;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

class IsOddMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 1;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

class IsZeroMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object == 0;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

template<class T>
class IsAlmostEqualMatcher: public Matcher {
public:
    explicit IsAlmostEqualMatcher(const T& target, const double& eps):
            target(target), eps(eps) {}

    bool matches(const T& object) {
        return fabs(object - target) < eps;
    }

    void describe(Description& description) override {
        description << "a number within " << eps << " of " << target;
    }

    void describeMismatch(Description& description) override {
        description << "a number not within " << eps << " of " << target;
    }
private:
    const T& target;
    double eps;
};

}

#endif
