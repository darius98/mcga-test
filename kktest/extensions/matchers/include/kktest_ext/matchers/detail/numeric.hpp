#pragma once

#include <kktest_ext/matchers/matcher.hpp>

namespace kktest::matchers::detail {

class IsPositiveMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object > 0;
    }

    void describe(Description* description) const override {
        (*description) << "a positive number";
    }

    void describeFailure(Description* description) const override {
        (*description) << "non-positive";
    }
};

class IsNegativeMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object < 0;
    }

    void describe(Description* description) const override {
        (*description) << "a negative number";
    }

    void describeFailure(Description* description) const override {
        (*description) << "non-negative";
    }
};

class IsEvenMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object % 2 == 0;
    }

    void describe(Description* description) const override {
        (*description) << "an even number";
    }

    void describeFailure(Description* description) const override {
        (*description) << "odd";
    }
};

class IsOddMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object % 2 == 1 || object % 2 == -1;
    }

    void describe(Description* description) const override {
        (*description) << "an odd number";
    }

    void describeFailure(Description* description) const override {
        (*description) << "even";
    }
};

class IsZeroMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object == 0;
    }

    void describe(Description* description) const override {
        (*description) << "zero";
    }

    void describeFailure(Description* description) const override {
        (*description) << "non-zero";
    }
};

class IsNotZeroMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object != 0;
    }

    void describe(Description* description) const override {
        (*description) << "non-zero";
    }

    void describeFailure(Description* description) const override {
        (*description) << "zero";
    }
};

template<class T>
class IsAlmostEqualMatcher: public StatelessMatcher {
 public:
    explicit IsAlmostEqualMatcher(const T& target, const double& eps):
            target(target), eps(eps) {}

    bool matches(const T& object) const {
        T delta = object - target;
        return -eps < delta && delta < eps;
    }

    void describe(Description* description) const override {
        (*description) << "a number within " << eps << " of " << target;
    }

    void describeFailure(Description* description) const override {
        (*description) << "a number not within " << eps << " of " << target;
    }

 private:
    const T& target;
    double eps;
};

}
