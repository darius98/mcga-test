#ifndef RUNTIME_TESTING_MATCHER_FUNCTIONAL_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_FUNCTIONAL_MATCHERS_H_

#include <cxxabi.h>

#include "matcher.hpp"


namespace matcher {

class ThrowsAnythingMatcher: public Matcher<std::function<void()>> {
public:
    bool matches(const std::function<void()>& func) override;

    void describe(const std::function<void()>& func,
                  Description* description) override;
};

template<class E>
class ThrowsSpecificMatcher: public Matcher<std::function<void()>> {
public:
    bool matches(const std::function<void()>& func) override {
        try {
            func();
            this->failureType = 1;
            return false;
        } catch(const E& exception) {
            this->failureType = 0;
            return true;
        } catch(...) {
            this->failureType = 2;
            return false;
        }
    }

    void describe(const std::function<void()>& func,
                  Description* description) override {
        description->append(
                "function that throws '",
                this->getErrorName(),
                "', "
        );
        if (failureType == 1) {
            description->append("got function that did not throw.");
        }
        if (failureType == 2) {
            description->append("got function that throws something else.");
        }
    }

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

    int failureType = -1;
};

}

#endif
