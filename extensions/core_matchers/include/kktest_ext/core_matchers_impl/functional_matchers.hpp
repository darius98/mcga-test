#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_FUNCTIONAL_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_FUNCTIONAL_MATCHERS_HPP_

#include <functional>

#include <kktest_ext/core_matchers_impl/matcher.hpp>
#include <kktest_ext/core_matchers_impl/detail/std_invoke_polyfill.hpp>

namespace kktest {
namespace core_matchers {

class ThrowsAnythingMatcher: public Matcher {
 public:
    bool matches(const std::function<void()>& func);

    void describe(Description* description);

    void describeMismatch(Description* description,
                          const std::function<void()>& func);
};

extern ThrowsAnythingMatcher throws;

template<class E>
class ThrowsSpecificMatcher: public Matcher {
 public:
    bool matches(const std::function<void()>& func) {
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
        (*description) << "a function that throws ";
        description->appendType<E>();
    }

    void describeMismatch(Description* description,
                          const std::function<void()>& func) {
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

template<class E>
ThrowsSpecificMatcher<E> throwsA() {
    return ThrowsSpecificMatcher<E>();
}

template<class F, class... Args>
std::function<void()> wrapFunc(const F& func, const Args... args) {
    return [&]() { detail::invokePolyfill(func, args...); };
}

}
}

#endif
