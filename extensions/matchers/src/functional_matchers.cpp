#include "extensions/matchers/include/kktest_ext/matchers_impl/functional_matchers.hpp"

#include "core/src/export.hpp"

using namespace std;

namespace kktest {
namespace matchers {

KKTEST_EXPORT detail::ThrowsAnythingMatcher throws;

namespace detail {

KKTEST_EXPORT bool ThrowsAnythingMatcher::matches(
        const function<void()>& func) {
    try {
        func();
        return false;
    } catch(...) {
        return true;
    }
}

KKTEST_EXPORT void ThrowsAnythingMatcher::describe(Description* description) {
    (*description) << "a function that throws";
}

KKTEST_EXPORT void ThrowsAnythingMatcher::describeMismatch(
        Description* description, const function<void()>& func) {
    (*description) << "a function that did not throw";
}

}
}
}
