#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_FUNCTIONAL_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_FUNCTIONAL_MATCHERS_HPP_

#include <functional>

#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>
#include <kktest_ext/matchers_impl/detail/std_invoke_polyfill.hpp>

namespace kktest {
namespace matchers {

template<class E>
detail::ThrowsSpecificMatcher<E> throwsA() {
    return detail::ThrowsSpecificMatcher<E>();
}

template<class F, class... Args>
Executable wrapFunc(const F& func, const Args... args) {
    return [&]() { detail::invokePolyfill(func, args...); };
}

namespace detail {

class ThrowsAnythingMatcher: public Matcher {
 public:
    bool matches(const Executable& func) {
        try {
            func();
            return false;
        } catch(...) {
            return true;
        }
    }

    void describe(Description* description) {
        (*description) << "a function that throws";
    }

    void describeMismatch(Description* description, const Executable& func) {
        (*description) << "a function that did not throw";
    }
};

template<class E>
class ThrowsSpecificMatcher: public Matcher {
 public:
    bool matches(const Executable& func) {
        try {
            func();
            failureType = 1;
            return false;
        } catch(const E& exception) {
            failureType = 0;
            return true;
        } catch(...) {
            failureType = 2;
            return false;
        }
    }

    void describe(Description* description) {
        (*description) << "a function that throws " << typeid(E).name();
    }

    void describeMismatch(Description* description, const Executable& func) {
        if (failureType == 1) {
            (*description) << "a function that did not throw";
        }
        if (failureType == 2) {
            (*description) << "a function that throws something else";
        }
    }

 private:
    int failureType = -1;
};

}

static detail::ThrowsAnythingMatcher throws;

}
}

#endif
