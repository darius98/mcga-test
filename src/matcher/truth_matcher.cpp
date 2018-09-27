#include "truth_matcher.hpp"

namespace matcher {

bool IsTrueMatcher::matches(const bool& object) {
    return object;
}

void IsTrueMatcher::describe(Description* description) {
    description->append("true");
}

void IsTrueMatcher::describeMismatch(Description* description) {
    description->append("false");
}

bool IsFalseMatcher::matches(const bool& object) {
    return !object;
}

void IsFalseMatcher::describe(Description* description) {
    description->append("false");
}

void IsFalseMatcher::describeMismatch(Description* description) {
    description->append("true");
}

}
