#include "extensions/matchers/include/kktest_ext/matchers_impl/iterable_matchers.hpp"

#include "core/src/export.hpp"

namespace kktest {
namespace matchers {

KKTEST_EXPORT detail::IsEmptyMatcher isEmpty;

KKTEST_EXPORT detail::IsNotEmptyMatcher isNotEmpty;

namespace detail {

KKTEST_EXPORT void IsEmptyMatcher::describe(Description* description) {
    (*description) << "empty iterable";
}

KKTEST_EXPORT void IsNotEmptyMatcher::describe(Description* description) {
    (*description) << "non-empty iterable";
}

}
}
}
