#ifndef MATCHER_CORE_MATCHERS_H_
#define MATCHER_CORE_MATCHERS_H_

#include <set>

#include "comparison_matcher.hpp"
#include "truth_matcher.hpp"


namespace matcher {

static const Matcher<bool>* isTrue = new TruthMatcher<true>();

static const Matcher<bool>* isFalse = new TruthMatcher<false>();

template<class T>
Matcher<T>* equal(const T& object) {
    return new ComparisonMatcher<T>(object, std::equal_to<T>(), "expected");
}

template<class T>
Matcher<T>* identical(const T& object) {
    return new ComparisonMatcher<T>(object, [](const T& a, const T& b) -> bool {
        return &a == &b;
    }, "expected variable at address");
}

template<class T>
Matcher<T>* lessThan(const T& object) {
    return new ComparisonMatcher<T>(
            object, std::less<T>(), "expected less than"
    );
}

template<class T>
Matcher<T>* lessThanOrEqualTo(const T& object) {
    return new ComparisonMatcher<T>(
            object, std::less_equal<T>(), "expected less than or equal to"
    );
}

template<class T>
Matcher<T>* greaterThan(const T& object) {
    return new ComparisonMatcher<T>(
            object, std::greater<T>(), "expected greater than"
    );
}

template<class T>
Matcher<T>* greaterThanOrEqualTo(const T& object) {
    return new ComparisonMatcher<T>(
            object, std::greater_equal<T>(), "expected greater than or equal to"
    );
}

} // namespace matcher

#endif
