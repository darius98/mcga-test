#include "truth_matcher.hpp"

namespace matcher {

bool IsTrueMatcher::matches(const bool& object) {
    return object;
}

void IsTrueMatcher::describeExpectation(Description* description) {
    description->append("true");
}

void IsTrueMatcher::describeFailure(const bool& object,
                                    Description* description) {
    description->append("false");
}

void IsTrueMatcher::describeSuccess(const bool& object,
                                    Description* description) {
    description->append("true");
}

bool IsFalseMatcher::matches(const bool& object) {
    return !object;
}

void IsFalseMatcher::describeExpectation(Description* description) {
    description->append("false");
}

void IsFalseMatcher::describeFailure(const bool& object,
                                     Description* description) {
    description->append("true");
}

void IsFalseMatcher::describeSuccess(const bool& object,
                                     Description* description) {
    description->append("false");
}

}
