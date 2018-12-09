#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_NUMERIC_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_NUMERIC_MATCHERS_HPP_

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {
namespace core_matchers {

class IsPositiveMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object > 0;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

extern IsPositiveMatcher isPositive;

class IsNegativeMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object < 0;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

extern IsNegativeMatcher isNegative;

class IsEvenMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 0;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

extern IsEvenMatcher isEven;

class IsOddMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 1 || object % 2 == -1;
    }

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

extern IsOddMatcher isOdd;

class IsZeroMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object == 0;
    }

    void describe(Description* description) override;
};

extern IsZeroMatcher isZero;

template<class T>
class IsAlmostEqualMatcher: public Matcher {
 public:
    explicit IsAlmostEqualMatcher(const T& target, const double& eps):
            target(target), eps(eps) {}

    bool matches(const T& object) {
        T delta = object - target;
        return -eps < delta && delta < eps;
    }

    void describe(Description* description) override {
        (*description) << "a number within " << eps << " of " << target;
    }

    void describeMismatch(Description* description) override {
        (*description) << "a number not within " << eps << " of " << target;
    }

 private:
    T target;
    double eps;
};

template<class T>
IsAlmostEqualMatcher<T> isAlmostEqualTo(const T& target, const double& eps) {
    return IsAlmostEqualMatcher<T>(target, eps);
}

}  // namespace core_matchers
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_NUMERIC_MATCHERS_HPP_
