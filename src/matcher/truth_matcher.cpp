#include "truth_matcher.hpp"

namespace matcher {

bool IsTrueMatcher::matches(const bool& object) {
    return object;
}

void IsTrueMatcher::describeExpectation(Description* description) {
    description->append("true");
}

void IsTrueMatcher::describeFailure(Description* description) {
    description->append("false");
}

void IsTrueMatcher::describeSuccess(Description* description) {
    description->append("true");
}

bool IsFalseMatcher::matches(const bool& object) {
    return !object;
}

void IsFalseMatcher::describeExpectation(Description* description) {
    description->append("false");
}

void IsFalseMatcher::describeFailure(Description* description) {
    description->append("true");
}

void IsFalseMatcher::describeSuccess(Description* description) {
    description->append("false");
}

}
