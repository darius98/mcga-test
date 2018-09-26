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

}
