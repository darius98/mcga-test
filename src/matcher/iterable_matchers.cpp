#include "iterable_matchers.hpp"


namespace matcher {

void IsEmptyMatcher::describe(Description* description) {
    description->append("empty iterable");
}

void IsEmptyMatcher::describeMismatch(Description* description) {
    description->append("non-empty iterable");
}

void IsNotEmptyMatcher::describe(Description* description) {
    description->append("non-empty iterable");
}

void IsNotEmptyMatcher::describeMismatch(Description* description) {
    description->append("empty iterable");
}

}
