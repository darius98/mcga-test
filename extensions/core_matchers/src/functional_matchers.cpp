#include <kktest_ext/core_matchers_impl/functional_matchers.hpp>

using namespace std;

namespace kktest {
namespace core_matchers {

bool ThrowsAnythingMatcher::matches(const function<void()>& func) {
    try {
        func();
        return false;
    } catch(...) {
        return true;
    }
}

void ThrowsAnythingMatcher::describe(Description* description) {
    (*description) << "a function that throws";
}

void ThrowsAnythingMatcher::describeMismatch(Description* description,
                                             const function<void()>& func) {
    (*description) << "a function that did not throw";
}

ThrowsAnythingMatcher throws;

}
}
