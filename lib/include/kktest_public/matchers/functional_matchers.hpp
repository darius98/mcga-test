#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_FUNCTIONAL_MATCHERS_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_FUNCTIONAL_MATCHERS_H_

#include <functional>

#include <kktest_public/core/matcher.hpp>


namespace kktest {

class ThrowsAnythingMatcher: public Matcher {
public:
    bool matches(const std::function<void()>& func);

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

extern ThrowsAnythingMatcher* throws;

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

    void describe(Description& description) override {
        description << "a function that throws ";
        description.appendType<E>();
    }

    void describeMismatch(Description& description) override {
        if (failureType == 1) {
            description << "a function that did not throw";
        }
        if (failureType == 2) {
            description << "a function that throws something else";
        }
    }
private:
    int failureType = -1;
};

template<class E>
ThrowsSpecificMatcher<E>* throwsA() {
    return new ThrowsSpecificMatcher<E>();
}

}

#endif
