#include "core_matchers.hpp"

using namespace std;


namespace matcher {

IsTrueMatcher* isTrue = new IsTrueMatcher();

IsFalseMatcher* isFalse = new IsFalseMatcher();

IsEmptyMatcher* isEmpty = new IsEmptyMatcher();

IsNotEmptyMatcher* isNotEmpty = new IsNotEmptyMatcher();

Matcher<void*>* isNull = new IsNullptrMatcher();

Matcher<void*>* isNotNull = new IsNotNullptrMatcher();

Matcher<function<void()>>* throws = new ThrowsAnythingMatcher();

IsPositiveMatcher* isPositive = new IsPositiveMatcher();

IsNegativeMatcher* isNegative = new IsNegativeMatcher();

IsEvenMatcher* isEven = new IsEvenMatcher();

IsOddMatcher* isOdd = new IsOddMatcher();

IsZeroMatcher* isZero = new IsZeroMatcher();

}
