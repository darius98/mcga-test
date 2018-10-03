#include "matcher/iterable_matchers.hpp"


namespace ktest {

void IsEmptyMatcher::describe(Description& description) {
    description << "empty iterable";
}

void IsEmptyMatcher::describeMismatch(Description& description) {
    description << "non-empty iterable";
}

void IsNotEmptyMatcher::describe(Description& description) {
    description << "non-empty iterable";
}

void IsNotEmptyMatcher::describeMismatch(Description& description) {
    description << "empty iterable";
}

}
