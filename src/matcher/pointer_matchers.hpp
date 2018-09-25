#ifndef RUNTIME_TESTING_MATCHER_POINTER_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_POINTER_MATCHERS_H_

#include "matcher.hpp"

namespace matcher {

class IsNullptrMatcher: public Matcher<void*> {
public:
    bool matches(void*const& obj) override {
        return obj == nullptr;
    }

    void describe(void*const& obj, Description* description) override {
        description->append("nullptr, got '", obj, "'");
    }
};

class IsNotNullptrMatcher: public Matcher<void*> {
public:
    bool matches(void*const& obj) override {
        return obj != nullptr;
    }

    void describe(void*const& obj, Description* description) override {
        description->append("non-null pointer, got nullptr");
    }
};

}

#endif
