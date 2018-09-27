#include "numeric_matchers.hpp"


namespace matcher {

void IsPositiveMatcher::describe(Description* description) {
    description->append("a positive number");
}

void IsPositiveMatcher::describeMismatch(Description* description) {
    description->append("a non-positive number");
}

void IsNegativeMatcher::describe(Description* description) {
    description->append("a negative number");
}

void IsNegativeMatcher::describeMismatch(Description* description) {
    description->append("a non-negative number");
}

void IsEvenMatcher::describe(Description* description) {
    description->append("an even number");
}

void IsEvenMatcher::describeMismatch(Description* description) {
    description->append("an odd number");
}

void IsOddMatcher::describe(Description* description) {
    description->append("an odd number");
}

void IsOddMatcher::describeMismatch(Description* description) {
    description->append("an even number");
}

void IsZeroMatcher::describe(Description* description) {
    description->append("zero");
}

void IsZeroMatcher::describeMismatch(Description* description) {
    description->append("not zero");
}

}