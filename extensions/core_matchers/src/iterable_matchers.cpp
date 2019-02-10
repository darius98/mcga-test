#include <kktest_ext/core_matchers_impl/iterable_matchers.hpp>

namespace kktest {
namespace core_matchers {

IsEmptyMatcher isEmpty;

IsNotEmptyMatcher isNotEmpty;

void IsEmptyMatcher::describe(Description* description) {
    (*description) << "empty iterable";
}

void IsNotEmptyMatcher::describe(Description* description) {
    (*description) << "non-empty iterable";
}

}
}
