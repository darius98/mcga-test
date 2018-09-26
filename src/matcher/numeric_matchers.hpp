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

    void describeExpectation(Description* description) {
        description->append("a positive number");
    }

    template<class T>
    void describeFailure(const T& object, Description* description) {
        description->append("a non-positive number");
    }

    template<class T>
    void describeSuccess(const T& object, Description* description) {
        description->append("a positive number");
    }
};

class IsNegativeMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object < 0;
    }

    void describeExpectation(Description* description) {
        description->append("a negative number");
    }

    template<class T>
    void describeFailure(const T& object, Description* description) {
        description->append("a non-negative number");
    }

    template<class T>
    void describeSuccess(const T& object, Description* description) {
        description->append("a negative number");
    }
};

class IsEvenMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 0;
    }

    void describeExpectation(Description* description) {
        description->append("an even number");
    }

    template<class T>
    void describeFailure(const T& object, Description* description) {
        description->append("an odd number");
    }

    template<class T>
    void describeSuccess(const T& object, Description* description) {
        description->append("an even number");
    }
};

class IsOddMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 1;
    }

    void describeExpectation(Description* description) {
        description->append("an odd number");
    }

    template<class T>
    void describeFailure(const T& object, Description* description) {
        description->append("an even number");
    }

    template<class T>
    void describeSuccess(const T& object, Description* description) {
        description->append("an odd number");
    }
};

class IsZeroMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object == 0;
    }

    void describeExpectation(Description* description) {
        description->append("zero");
    }

    template<class T>
    void describeFailure(const T& object, Description* description) {
        description->append("not zero");
    }

    template<class T>
    void describeSuccess(const T& object, Description* description) {
        description->append("zero");
    }
};

template<class T>
class IsAlmostEqualMatcher: public Matcher<T> {
public:
    explicit IsAlmostEqualMatcher(const T& target, const double& eps):
            target(target), eps(eps) {}

    bool matches(const T& object) override {
        return fabs(object - target) < eps;
    }

    void describeExpectation(Description* description) override {
        description->append("a number within ", eps, " of ", target);
    }

    void describeSuccess(const T& obj, Description* description) override {
        description->append("a number within ", eps, " of ", target);
    }

    void describeFailure(const T& obj, Description* description) override {
        description->append("a number not within ", eps, " of ", target);
    }
private:
    const T& target;
    double eps;
};

}

#endif
