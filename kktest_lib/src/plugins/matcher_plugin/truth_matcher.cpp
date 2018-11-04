#include <kktest_matchers/truth_matcher.hpp>


namespace kktest {

bool IsTrueMatcher::matches(const bool& object) {
    return object;
}

void IsTrueMatcher::describe(Description& description) {
    description << "true";
}

void IsTrueMatcher::describeMismatch(Description& description) {
    description << "false";
}

IsTrueMatcher* isTrue = new IsTrueMatcher();

bool IsFalseMatcher::matches(const bool& object) {
    return !object;
}

void IsFalseMatcher::describe(Description& description) {
    description << "false";
}

void IsFalseMatcher::describeMismatch(Description& description) {
    description << "true";
}

IsFalseMatcher* isFalse = new IsFalseMatcher();

}
