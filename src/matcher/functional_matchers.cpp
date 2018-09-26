#include "functional_matchers.hpp"

using namespace std;

namespace matcher {

bool ThrowsAnythingMatcher::matches(const function<void()>& func) {
    try {
        func();
        return false;
    } catch(...) {
        return true;
    }
}

void ThrowsAnythingMatcher::describe(const function<void()>& func,
                                     Description* description) {
    description->append("function that throws, got "
                        "function that did not throw.");
}

}
