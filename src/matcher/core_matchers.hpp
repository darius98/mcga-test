#ifndef RUNTIME_TESTING_MATCHER_CORE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_CORE_MATCHERS_H_

#include <set>

#include "comparison_matcher.hpp"
#include "iterable_matcher.hpp"
#include "truth_matcher.hpp"


namespace matcher {

static Matcher<bool>* isTrue = new TruthMatcher<true>();

static Matcher<bool>* isFalse = new TruthMatcher<false>();

template<class T>
Matcher<T>* equal(const T& object) {
    return new ComparisonMatcher<T>(object, std::equal_to<T>(), "");
}

template<class T>
Matcher<T>* identical(const T& object) {
    return new ComparisonMatcher<T>(object, [](const T& a, const T& b) -> bool {
        return &a == &b;
    }, "variable at address ");
}

template<class T>
Matcher<T>* lessThan(const T& object) {
    return new ComparisonMatcher<T>(
            object, std::less<T>(), "less than "
    );
}

template<class T>
Matcher<T>* lessThanOrEqualTo(const T& object) {
    return new ComparisonMatcher<T>(
            object, std::less_equal<T>(), "less than or equal to "
    );
}

template<class T>
Matcher<T>* greaterThan(const T& object) {
    return new ComparisonMatcher<T>(
            object, std::greater<T>(), "greater than "
    );
}

template<class T>
Matcher<T>* greaterThanOrEqualTo(const T& object) {
    return new ComparisonMatcher<T>(
            object, std::greater_equal<T>(), "greater than or equal to "
    );
}

static IsEmptyMatcher* isEmpty = new IsEmptyMatcher();

static IsNotEmptyMatcher* isNotEmpty = new IsNotEmptyMatcher();

template<class SizeMatcher>
CollectionSizeMatcher<SizeMatcher>* hasSize(SizeMatcher* sizeMatcher) {
    return new CollectionSizeMatcher<SizeMatcher>(sizeMatcher);
}

template<class T>
CollectionSizeMatcher<Matcher<T>>* hasSize(const T& object) {
    return new CollectionSizeMatcher<Matcher<T>>(equal(object));
}

} // namespace matcher

#endif
