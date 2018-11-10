#include <matchers/functional_matchers.hpp>

using namespace std;


namespace kktest {

bool ThrowsAnythingMatcher::matches(const function<void()>& func) {
    try {
        func();
        return false;
    } catch(...) {
        return true;
    }
}

void ThrowsAnythingMatcher::describe(Description& description) {
    description << "a function that throws";
}

void ThrowsAnythingMatcher::describeMismatch(Description& description) {
    description << "a function that did not throw";
}

ThrowsAnythingMatcher* throws = new ThrowsAnythingMatcher();

}
