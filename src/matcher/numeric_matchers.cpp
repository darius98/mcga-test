#include "numeric_matchers.hpp"


namespace ktest {

void IsPositiveMatcher::describe(Description& description) {
    description << "a positive number";
}

void IsPositiveMatcher::describeMismatch(Description& description) {
    description << "a non-positive number";
}

void IsNegativeMatcher::describe(Description& description) {
    description << "a negative number";
}

void IsNegativeMatcher::describeMismatch(Description& description) {
    description << "a non-negative number";
}

void IsEvenMatcher::describe(Description& description) {
    description << "an even number";
}

void IsEvenMatcher::describeMismatch(Description& description) {
    description << "an odd number";
}

void IsOddMatcher::describe(Description& description) {
    description << "an odd number";
}

void IsOddMatcher::describeMismatch(Description& description) {
    description << "an even number";
}

void IsZeroMatcher::describe(Description& description) {
    description << "zero";
}

void IsZeroMatcher::describeMismatch(Description& description) {
    description << "not zero";
}

}