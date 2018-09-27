#ifndef RUNTIME_TESTING_MATCHER_FUNCTIONAL_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_FUNCTIONAL_MATCHERS_H_

#include <cxxabi.h>
#include <functional>

#include "matcher.hpp"


namespace matcher {

class ThrowsAnythingMatcher: public Matcher<std::function<void()>> {
public:
    bool matches(const std::function<void()>& func) override;

    void describe(Description* description) override;

    void describeMismatch(Description* description) override;
};

template<class E>
class ThrowsSpecificMatcher: public Matcher<std::function<void()>> {
private:
    static std::string getErrorName() {
        int stat;
        std::string rawName = typeid(E).name();
        char* name = abi::__cxa_demangle(
                rawName.c_str(), nullptr, nullptr, &stat
        );
        if(stat == 0) {
            rawName = name;
            free(name);
        }
        return rawName;
    }
public:
    bool matches(const std::function<void()>& func) override {
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
        description->append("a function that throws ", getErrorName());
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
