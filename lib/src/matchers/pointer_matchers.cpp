#include <matchers/pointer_matchers.hpp>


namespace kktest {

bool IsNullptrMatcher::matches(void*const& obj) {
    return obj == nullptr;
}

void IsNullptrMatcher::describe(Description& description) {
    description << "nullptr";
}

void IsNullptrMatcher::describeMismatch(Description& description) {
    description << "non-null pointer";
}

IsNullptrMatcher isNull;

bool IsNotNullptrMatcher::matches(void*const& obj) {
    return obj != nullptr;
}

void IsNotNullptrMatcher::describe(Description& description) {
    description << "non-null pointer";
}

void IsNotNullptrMatcher::describeMismatch(Description& description) {
    description << "nullptr";
}

IsNotNullptrMatcher isNotNull;

}
