#include "matcher/pointer_matchers.hpp"


namespace ktest {

bool IsNullptrMatcher::matches(void*const& obj) {
    return obj == nullptr;
}

void IsNullptrMatcher::describe(Description& description) {
    description << "nullptr";
}

void IsNullptrMatcher::describeMismatch(Description& description) {
    description << "non-null pointer";
}

bool IsNotNullptrMatcher::matches(void*const& obj) {
    return obj != nullptr;
}

void IsNotNullptrMatcher::describe(Description& description) {
    description << "non-null pointer";
}

void IsNotNullptrMatcher::describeMismatch(Description& description) {
    description << "nullptr";
}

}
