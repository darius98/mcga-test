#ifndef RUNTIME_TESTING_MATCHER_CORE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_CORE_MATCHERS_H_

#include <set>

#include "comparison_matchers.hpp"
#include "composite_matchers.hpp"
#include "functional_matchers.hpp"
#include "iterable_matchers.hpp"
#include "numeric_matchers.hpp"
#include "pointer_matchers.hpp"
#include "truth_matcher.hpp"


namespace matcher {

extern IsTrueMatcher* isTrue;

extern IsFalseMatcher* isFalse;

template<class T>
ComparisonMatcher<T>* isEqualTo(const T& object) {
    return new ComparisonMatcher<T>(object, std::equal_to<T>(), "");
}

template<class T>
ComparisonMatcher<T>* isIdenticalTo(const T& object) {
    return new ComparisonMatcher<T>(object, [](const T& a, const T& b) -> bool {
        return &a == &b;
    }, "variable at address ");
}

template<class T>
ComparisonMatcher<T>* isLessThan(const T &object) {
    return new ComparisonMatcher<T>(
            object, std::less<T>(), "less than "
    );
}

template<class T>
ComparisonMatcher<T>* isLessThanOrEqualTo(const T &object) {
    return new ComparisonMatcher<T>(
            object, std::less_equal<T>(), "less than or equal to "
    );
}

template<class T>
ComparisonMatcher<T>* isGreaterThan(const T &object) {
    return new ComparisonMatcher<T>(
            object, std::greater<T>(), "greater than "
    );
}

template<class T>
ComparisonMatcher<T>* isGreaterThanOrEqualTo(const T &object) {
    return new ComparisonMatcher<T>(
            object, std::greater_equal<T>(), "greater than or equal to "
    );
}

extern IsEmptyMatcher* isEmpty;

extern IsNotEmptyMatcher* isNotEmpty;

template<class SizeMatcher, IS_MATCHER(SizeMatcher)>
IterableSizeMatcher<SizeMatcher>* hasSize(SizeMatcher* m) {
    return new IterableSizeMatcher<SizeMatcher>(m);
}

template<class T>
IterableSizeMatcher<ComparisonMatcher<T>>* hasSize(const T& object) {
    return new IterableSizeMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

template<class ElementMatcher, IS_MATCHER(ElementMatcher)>
IterableEachMatcher<ElementMatcher>* eachElement(ElementMatcher *m) {
    return new IterableEachMatcher<ElementMatcher>(m);
}

template<class T>
IterableEachMatcher<ComparisonMatcher<T>>* eachElement(const T &object) {
    return new IterableEachMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

template<class ElementMatcher, IS_MATCHER(ElementMatcher)>
IterableAnyMatcher<ElementMatcher>* anyElement(ElementMatcher *m) {
    return new IterableAnyMatcher<ElementMatcher>(m);
}

template<class T>
IterableAnyMatcher<ComparisonMatcher<T>>* anyElement(const T &object) {
    return new IterableAnyMatcher<ComparisonMatcher<T>>(isEqualTo(object));
}

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
AndMatcher<M1, M2>* both(M1* m1, M2* m2) {
    return new AndMatcher<M1, M2>(m1, m2);
}

template<class M1, class M2, IS_MATCHER(M1), IS_MATCHER(M2)>
OrMatcher<M1, M2>* either(M1* m1, M2* m2) {
    return new OrMatcher<M1, M2>(m1, m2);
}

template<class M, IS_MATCHER(M)>
NotMatcher<M>* isNot(M* matcher) {
    return new NotMatcher<M>(matcher);
}

extern IsNullptrMatcher* isNull;

extern IsNotNullptrMatcher* isNotNull;

extern ThrowsAnythingMatcher* throws;

template<class E>
ThrowsSpecificMatcher<E>* throwsA() {
    return new ThrowsSpecificMatcher<E>();
}

extern IsPositiveMatcher* isPositive;

extern IsNegativeMatcher* isNegative;

extern IsEvenMatcher* isEven;

extern IsOddMatcher* isOdd;

extern IsZeroMatcher* isZero;

template<class T>
IsAlmostEqualMatcher<T>* isAlmostEqualTo(const T& target,
                                         const double& eps=0.000001) {
    return new IsAlmostEqualMatcher<T>(target, eps);
}

} // namespace matcher

#endif
