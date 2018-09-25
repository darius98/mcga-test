#include "truth_matcher.hpp"

namespace matcher {

bool IsTrueMatcher::matches(const bool& object) {
    return object;
}

void IsTrueMatcher::describe(const bool& object, Description* description) {
    description->append("true, got false");
}

bool IsFalseMatcher::matches(const bool& object) {
    return !object;
}

void IsFalseMatcher::describe(const bool& object, Description* description) {
    description->append("false, got true");
}

}
