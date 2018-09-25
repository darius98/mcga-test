#include "core_matchers.hpp"

namespace matcher {

IsTrueMatcher* isTrue = new IsTrueMatcher();

IsFalseMatcher* isFalse = new IsFalseMatcher();

IsEmptyMatcher* isEmpty = new IsEmptyMatcher();

IsNotEmptyMatcher* isNotEmpty = new IsNotEmptyMatcher();

Matcher<void*>* isNull = new IsNullptrMatcher();

Matcher<void*>* isNotNull = new IsNotNullptrMatcher();

}
