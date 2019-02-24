#include "kktest/extensions/matchers/include/kktest_ext/matchers_impl/pointer_matchers.hpp"

namespace kktest {
namespace matchers {

detail::IsNullptrMatcher isNull;

detail::IsNotNullptrMatcher isNotNull;

namespace detail {

bool IsNullptrMatcher::matches(void*const& obj) {
    return obj == nullptr;
}

void IsNullptrMatcher::describe(Description* description) {
    (*description) << "nullptr";
}

void IsNullptrMatcher::describeMismatch(Description* description, void*const&) {
    (*description) << "non-null pointer";
}

bool IsNotNullptrMatcher::matches(void*const& obj) {
    return obj != nullptr;
}

void IsNotNullptrMatcher::describe(Description* description) {
    (*description) << "non-null pointer";
}

void IsNotNullptrMatcher::describeMismatch(Description* description,
                                           void*const&) {
    (*description) << "nullptr";
}

}
}
}
