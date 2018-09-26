#include "pointer_matchers.hpp"


namespace matcher {

bool IsNullptrMatcher::matches(void*const& obj) {
    return obj == nullptr;
}

void IsNullptrMatcher::describeExpectation(Description* description) {
    description->append("nullptr");
}

void IsNullptrMatcher::describeFailure(Description* description) {
    description->append("non-null pointer");
}

bool IsNotNullptrMatcher::matches(void*const& obj) {
    return obj != nullptr;
}

void IsNotNullptrMatcher::describeExpectation(Description* description) {
    description->append("non-null pointer");
}

void IsNotNullptrMatcher::describeFailure(Description* description) {
    description->append("nullptr");
}

}
