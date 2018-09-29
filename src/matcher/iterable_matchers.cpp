#include "iterable_matchers.hpp"


namespace matcher {

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
