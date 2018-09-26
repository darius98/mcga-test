#include "size_matchers.hpp"


namespace matcher {

void IsEmptyMatcher::describeExpectation(Description* description) {
    description->append("empty iterable");
}

void IsEmptyMatcher::describeFailure(Description* description) {
    description->append("non-empty iterable");
}

void IsEmptyMatcher::describeSuccess(Description* description) {
    description->append("empty iterable");
}

void IsNotEmptyMatcher::describeExpectation(Description* description) {
    description->append("non-empty iterable");
}

void IsNotEmptyMatcher::describeFailure(Description* description) {
    description->append("empty iterable");
}

void IsNotEmptyMatcher::describeSuccess(Description* description) {
    description->append("non-empty iterable");
}

}