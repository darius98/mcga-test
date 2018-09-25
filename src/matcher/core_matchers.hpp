#ifndef RUNTIME_TESTING_MATCHER_CORE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_CORE_MATCHERS_H_

#include <set>

#include "comparison_matcher.hpp"
#include "composite_matchers.hpp"
#include "pointer_matchers.hpp"
#include "size_matchers.hpp"
#include "truth_matcher.hpp"


namespace matcher {

extern IsTrueMatcher* isTrue;

extern IsFalseMatcher* isFalse;

template<class T>
Matcher<T>* isEqualTo(const T& object) {
    return new ComparisonMatcher<T>(object, std::equal_to<T>(), "");
}

template<class T>
Matcher<T>* isIdenticalTo(const T& object) {
    return new ComparisonMatcher<T>(object, [](const T& a, const T& b) -> bool {
        return &a == &b;
    }, "variable at address ");
}

template<class T>
Matcher<T>* isLessThan(const T &object) {
    return new ComparisonMatcher<T>(
            object, std::less<T>(), "less than "
    );
}

template<class T>
Matcher<T>* isLessThanOrEqualTo(const T &object) {
    return new ComparisonMatcher<T>(
            object, std::less_equal<T>(), "less than or equal to "
    );
}

template<class T>
Matcher<T>* isGreaterThan(const T &object) {
    return new ComparisonMatcher<T>(
            object, std::greater<T>(), "greater than "
    );
}

template<class T>
Matcher<T>* isGreaterThanOrEqualTo(const T &object) {
    return new ComparisonMatcher<T>(
            object, std::greater_equal<T>(), "greater than or equal to "
    );
}

extern IsEmptyMatcher* isEmpty;

extern IsNotEmptyMatcher* isNotEmpty;

template<class SizeMatcher>
CollectionSizeMatcher<SizeMatcher>* hasSize(SizeMatcher* sizeMatcher) {
    return new CollectionSizeMatcher<SizeMatcher>(sizeMatcher);
}

template<class T>
CollectionSizeMatcher<Matcher<T>>* hasSize(const T& object) {
    return new CollectionSizeMatcher<Matcher<T>>(isEqualTo(object));
}

template<class M1, class M2>
AndMatcher<M1, M2>* both(M1* m1, M2* m2) {
    return new AndMatcher<M1, M2>(m1, m2);
}

template<class M1, class M2>
OrMatcher<M1, M2>* either(M1* m1, M2* m2) {
    return new OrMatcher<M1, M2>(m1, m2);
}

template<class Matcher>
NotMatcher<Matcher>* isNot(Matcher* matcher) {
    return new NotMatcher<Matcher>(matcher);
}

extern Matcher<void*>* isNull;

extern Matcher<void*>* isNotNull;

} // namespace matcher

#endif
