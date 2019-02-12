#include <kktest_ext/core_matchers_impl/iterable_matchers.hpp>

namespace kktest {
namespace core_matchers {

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
