#include "pointer_matchers.hpp"


namespace matcher {

bool IsNullptrMatcher::matches(void*const& obj) {
    return obj == nullptr;
}

void IsNullptrMatcher::describeExpectation(Description* description) {
    description->append("nullptr");
}

void IsNullptrMatcher::describeFailure(void*const& obj,
                                       Description* description) {
    description->append("non-null pointer");
}

void IsNullptrMatcher::describeSuccess(void*const& obj,
                                       Description* description) {
    description->append("nullptr");
}

bool IsNotNullptrMatcher::matches(void*const& obj) {
    return obj != nullptr;
}

void IsNotNullptrMatcher::describeExpectation(Description* description) {
    description->append("non-null pointer");
}

void IsNotNullptrMatcher::describeFailure(void*const& obj,
                                          Description* description) {
    description->append("nullptr");
}

void IsNotNullptrMatcher::describeSuccess(void*const& obj,
                                          Description* description) {
    description->append("non-null pointer");
}

}
