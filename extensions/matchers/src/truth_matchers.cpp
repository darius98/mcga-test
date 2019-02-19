#include "extensions/matchers/include/kktest_ext/matchers_impl/truth_matchers.hpp"

#include "core/src/export.hpp"

namespace kktest {
namespace matchers {

KKTEST_EXPORT detail::IsTrueMatcher isTrue;

KKTEST_EXPORT detail::IsFalseMatcher isFalse;

namespace detail {

KKTEST_EXPORT bool IsTrueMatcher::matches(const bool& object) {
    return object;
}

KKTEST_EXPORT void IsTrueMatcher::describe(Description* description) {
    (*description) << "true";
}

KKTEST_EXPORT void IsTrueMatcher::describeMismatch(Description* description,
                                                   const bool&) {
    (*description) << "false";
}

KKTEST_EXPORT bool IsFalseMatcher::matches(const bool& object) {
    return !object;
}

KKTEST_EXPORT void IsFalseMatcher::describe(Description* description) {
    (*description) << "false";
}

KKTEST_EXPORT void IsFalseMatcher::describeMismatch(Description* description,
                                                    const bool&) {
    (*description) << "true";
}

}
}
}
