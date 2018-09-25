#include "pointer_matchers.hpp"


namespace matcher {

bool IsNullptrMatcher::matches(void*const& obj) {
    return obj == nullptr;
}

void IsNullptrMatcher::describe(void*const& obj, Description* description)  {
    description->append("nullptr, got '", obj, "'");
}

bool IsNotNullptrMatcher::matches(void*const& obj) {
    return obj != nullptr;
}

void IsNotNullptrMatcher::describe(void*const& obj, Description* description) {
    description->append("non-null pointer, got nullptr");
}

}
