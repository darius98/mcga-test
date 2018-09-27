#include "core_matchers.hpp"

using namespace std;


namespace matcher {

IsTrueMatcher* isTrue = new IsTrueMatcher();

IsFalseMatcher* isFalse = new IsFalseMatcher();

IsEmptyMatcher* isEmpty = new IsEmptyMatcher();

IsNotEmptyMatcher* isNotEmpty = new IsNotEmptyMatcher();

IsNullptrMatcher* isNull = new IsNullptrMatcher();

IsNotNullptrMatcher* isNotNull = new IsNotNullptrMatcher();

ThrowsAnythingMatcher* throws = new ThrowsAnythingMatcher();

IsPositiveMatcher* isPositive = new IsPositiveMatcher();

IsNegativeMatcher* isNegative = new IsNegativeMatcher();

IsEvenMatcher* isEven = new IsEvenMatcher();

IsOddMatcher* isOdd = new IsOddMatcher();

IsZeroMatcher* isZero = new IsZeroMatcher();

}
