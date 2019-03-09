#pragma once

#include <kktest_ext/matchers/matcher.hpp>

namespace kktest::matchers::detail {

class IsPositiveMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object > 0;
    }

    void describe(Description* description) const {
        (*description) << "a positive number";
    }

    void describeFailure(Description* description) const {
        (*description) << "non-positive";
    }
};

class IsNegativeMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object < 0;
    }

    void describe(Description* description) const {
        (*description) << "a negative number";
    }

    void describeFailure(Description* description) const {
        (*description) << "non-negative";
    }
};

class IsEvenMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object % 2 == 0;
    }

    void describe(Description* description) const {
        (*description) << "an even number";
    }

    void describeFailure(Description* description) const {
        (*description) << "odd";
    }
};

class IsOddMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object % 2 == 1 || object % 2 == -1;
    }

    void describe(Description* description) const {
        (*description) << "an odd number";
    }

    void describeFailure(Description* description) const {
        (*description) << "even";
    }
};

class IsZeroMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object == 0;
    }

    void describe(Description* description) const {
        (*description) << "zero";
    }

    void describeFailure(Description* description) const {
        (*description) << "non-zero";
    }
};

class IsNotZeroMatcher: public StatelessMatcher {
 public:
    template<class T>
    bool matches(const T& object) const {
        return object != 0;
    }

    void describe(Description* description) const {
        (*description) << "non-zero";
    }

    void describeFailure(Description* description) const {
        (*description) << "zero";
    }
};

template<class T>
class IsAlmostEqualMatcher: public StatelessMatcher {
 public:
    IsAlmostEqualMatcher(const T& target, const double& eps):
            target(target), eps(eps) {}

    bool matches(const T& object) const {
        T delta = object - target;
        return -eps < delta && delta < eps;
    }

    void describe(Description* description) const {
        (*description) << "a number within " << eps << " of " << target;
    }

    void describeFailure(Description* description) const {
        (*description) << "a number not within " << eps << " of " << target;
    }

 private:
    const T& target;
    double eps;
};

}
