#ifndef RUNTIME_TESTING_MATCHER_FUNCTIONAL_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_FUNCTIONAL_MATCHERS_H_

#include <cxxabi.h>
#include <functional>

#include "matcher.hpp"


namespace matcher {

class ThrowsAnythingMatcher: public Matcher {
public:
    bool matches(const std::function<void()>& func);

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

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

    void describe(Description* description) override {
        description->append("a function that throws ")->appendType<E>();
    }

    void describeMismatch(Description* description) override {
        if (failureType == 1) {
            description->append("a function that did not throw.");
        }
        if (failureType == 2) {
            description->append("a function that throws something else.");
        }
    }
private:
    int failureType = -1;
};

}

#endif
