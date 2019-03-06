#pragma once

// This file forward-declares all classes used from the public API of matchers.
// We use this to maintain the public API at the top of each respective matcher
// file.

namespace kktest::matchers::detail {

// Comparison matchers
template<class T> class EqualityMatcher;
template<class T> class NonEqualityMatcher;
template<class T> class IsLessThanMatcher;
template<class T> class IsLessThanEqualMatcher;
template<class T> class IsGreaterThanMatcher;
template<class T> class IsGreaterThanEqualMatcher;
template<class T> class IdentityMatcher;

// Composite matchers
template<class M1, class M2> class AndMatcher;
template<class M1, class M2> class OrMatcher;
template<class M> class NotMatcher;

// Functional matchers
class ThrowsAnythingMatcher;
template<class E> class ThrowsSpecificMatcher;

// Iterable matchers
class IsEmptyMatcher;
class IsNotEmptyMatcher;
template<class M> class IterableSizeMatcher;
template<class M> class IterableEachMatcher;
template<class M> class IterableAnyMatcher;

// Numeric matchers
class IsPositiveMatcher;
class IsNegativeMatcher;
class IsZeroMatcher;
class IsEvenMatcher;
class IsOddMatcher;
template<class T> class IsAlmostEqualMatcher;

// Pointer matchers
class IsNullptrMatcher;
class IsNotNullptrMatcher;

// String matchers
class CharInStringMatcher;
class IsSubstringMatcher;

// Truth matchers
class IsTrueMatcher;
class IsFalseMatcher;

}
