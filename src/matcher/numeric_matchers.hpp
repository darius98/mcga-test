#ifndef RUNTIME_TESTING_MATCHER_NUMERIC_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_NUMERIC_MATCHERS_H_

#include <cmath>
#include "matcher.hpp"


namespace matcher {

class IsPositiveMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object > 0;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

class IsNegativeMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object < 0;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

class IsEvenMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 0;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

class IsOddMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 1;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

class IsZeroMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object == 0;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

template<class T>
class IsAlmostEqualMatcher: public BaseMatcher {
public:
    explicit IsAlmostEqualMatcher(const T& target, const double& eps):
            target(target), eps(eps) {}

    bool matches(const T& object) {
        return fabs(object - target) < eps;
    }

    void describe(Description* description) override {
        description->append("a number within ", eps, " of ", target);
    }

    void describeMismatch(Description* description) override {
        description->append("a number not within ", eps, " of ", target);
    }
private:
    const T& target;
    double eps;
};

}

#endif
