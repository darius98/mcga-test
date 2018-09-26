#include "numeric_matchers.hpp"


namespace matcher {

void IsPositiveMatcher::describeExpectation(Description* description) {
    description->append("a positive number");
}

void IsPositiveMatcher::describeFailure(Description* description) {
    description->append("a non-positive number");
}

void IsPositiveMatcher::describeSuccess(Description* description) {
    description->append("a positive number");
}

void IsNegativeMatcher::describeExpectation(Description* description) {
    description->append("a negative number");
}

void IsNegativeMatcher::describeFailure(Description* description) {
    description->append("a non-negative number");
}

void IsNegativeMatcher::describeSuccess(Description* description) {
    description->append("a negative number");
}

void IsEvenMatcher::describeExpectation(Description* description) {
    description->append("an even number");
}

void IsEvenMatcher::describeFailure(Description* description) {
    description->append("an odd number");
}

void IsEvenMatcher::describeSuccess(Description* description) {
    description->append("an even number");
}

void IsOddMatcher::describeExpectation(Description* description) {
    description->append("an odd number");
}

void IsOddMatcher::describeFailure(Description* description) {
    description->append("an even number");
}

void IsOddMatcher::describeSuccess(Description* description) {
    description->append("an odd number");
}

void IsZeroMatcher::describeExpectation(Description* description) {
    description->append("zero");
}

void IsZeroMatcher::describeFailure(Description* description) {
    description->append("not zero");
}

void IsZeroMatcher::describeSuccess(Description* description) {
    description->append("zero");
}

}