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

    template<class T>
    void describe(const T& object, Description* description) {
        description->append("positive number, got '", object, "'");
    }
};

class IsNegativeMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object < 0;
    }

    template<class T>
    void describe(const T& object, Description* description) {
        description->append("negative number, got '", object, "'");
    }
};

class IsEvenMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 0;
    }

    template<class T>
    void describe(const T& object, Description* description) {
        description->append("even number, got '", object, "'");
    }
};

class IsOddMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 1;
    }

    template<class T>
    void describe(const T& object, Description* description) {
        description->append("odd number, got '", object, "'");
    }
};

class IsZeroMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object == 0;
    }

    template<class T>
    void describe(const T& object, Description* description) {
        description->append("zero, got '", object, "'");
    }
};

template<class T>
class IsAlmostEqualMatcher: public Matcher<T> {
public:
    explicit IsAlmostEqualMatcher(const T& target, const double& eps):
            target(target), eps(eps) {}

    bool matches(const T& object) override {
        return fabs(object - this->target) < this->eps;
    }

    void describe(const T& object, Description* description) {
        description->append(
            "number within ",
            this->eps,
            " of ",
            this->target,
            ", got ",
            object
        );
    }
private:
    const T& target;
    double eps;
};

}

#endif
