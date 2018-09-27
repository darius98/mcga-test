#include "pointer_matchers.hpp"


namespace matcher {

bool IsNullptrMatcher::matches(void*const& obj) {
    return obj == nullptr;
}

void IsNullptrMatcher::describe(Description* description) {
    description->append("nullptr");
}

void IsNullptrMatcher::describeMismatch(Description* description) {
    description->append("non-null pointer");
}

bool IsNotNullptrMatcher::matches(void*const& obj) {
    return obj != nullptr;
}

void IsNotNullptrMatcher::describe(Description* description) {
    description->append("non-null pointer");
}

void IsNotNullptrMatcher::describeMismatch(Description* description) {
    description->append("nullptr");
}

}
