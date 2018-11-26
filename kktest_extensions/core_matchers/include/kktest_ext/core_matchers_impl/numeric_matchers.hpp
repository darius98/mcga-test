#ifndef KKTEST_EXT_CORE_MATCHERS_IMPL_NUMERIC_MATCHERS_H_
#define KKTEST_EXT_CORE_MATCHERS_IMPL_NUMERIC_MATCHERS_H_

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {

class IsPositiveMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object > 0;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsPositiveMatcher isPositive;

class IsNegativeMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object < 0;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsNegativeMatcher isNegative;

class IsEvenMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 0;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsEvenMatcher isEven;

class IsOddMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 1 || object % 2 == -1;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsOddMatcher isOdd;

class IsZeroMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object == 0;
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern IsZeroMatcher isZero;

template<class T>
class IsAlmostEqualMatcher: public Matcher {
public:
    explicit IsAlmostEqualMatcher(const T& target, const double& eps): target(target), eps(eps) {}

    bool matches(const T& object) {
        T delta = object - target;
        return -eps < delta && delta < eps;
    }

    void describe(Description& description) override {
        description << "a number within " << eps << " of " << target;
    }

    void describeMismatch(Description& description) override {
        description << "a number not within " << eps << " of " << target;
    }
private:
    T target;
    double eps;
};

template<class T>
IsAlmostEqualMatcher<T> isAlmostEqualTo(const T& target, const double& eps=0.000001) {
    return IsAlmostEqualMatcher<T>(target, eps);
}

}

#endif
