#include "extensions/matchers/include/kktest_ext/matchers_impl/pointer_matchers.hpp"

#include "core/src/export.hpp"

namespace kktest {
namespace matchers {

KKTEST_EXPORT detail::IsNullptrMatcher isNull;

KKTEST_EXPORT detail::IsNotNullptrMatcher isNotNull;

namespace detail {

KKTEST_EXPORT bool IsNullptrMatcher::matches(void*const& obj) {
    return obj == nullptr;
}

KKTEST_EXPORT void IsNullptrMatcher::describe(Description* description) {
    (*description) << "nullptr";
}

KKTEST_EXPORT void IsNullptrMatcher::describeMismatch(Description* description,
                                                      void*const&) {
    (*description) << "non-null pointer";
}

KKTEST_EXPORT bool IsNotNullptrMatcher::matches(void*const& obj) {
    return obj != nullptr;
}

KKTEST_EXPORT void IsNotNullptrMatcher::describe(Description* description) {
    (*description) << "non-null pointer";
}

KKTEST_EXPORT void IsNotNullptrMatcher::describeMismatch(
        Description* description, void*const&) {
    (*description) << "nullptr";
}

}
}
}
