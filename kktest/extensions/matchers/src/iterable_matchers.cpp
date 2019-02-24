#include "kktest/extensions/matchers/include/kktest_ext/matchers_impl/iterable_matchers.hpp"

namespace kktest {
namespace matchers {

detail::IsEmptyMatcher isEmpty;

detail::IsNotEmptyMatcher isNotEmpty;

namespace detail {

void IsEmptyMatcher::describe(Description* description) {
    (*description) << "empty iterable";
}

void IsNotEmptyMatcher::describe(Description* description) {
    (*description) << "non-empty iterable";
}

}
}
}
