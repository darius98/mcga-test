#ifndef RUNTIME_TESTING_MATCHER_SIZE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_SIZE_MATCHERS_H_

#include "matcher.hpp"


namespace matcher {

class IsEmptyMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object.empty();
    }

    template<class T>
    void describe(const T& object, Description* description) {
        description->append("empty container, got '", object, "'");
    }
};

class IsNotEmptyMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return !object.empty();
    }

    template<class T>
    void describe(const T& object, Description* description) {
        description->append("non-empty container, got '", object, "'");
    }
};

template<class SizeMatcher, IS_MATCHER(SizeMatcher)>
class IterableSizeMatcher: public BaseMatcher {
public:
    explicit IterableSizeMatcher(SizeMatcher* sizeMatcher):
            sizeMatcher(sizeMatcher) {}

    template<class T>
    bool matches(const T& object) {
        return this->sizeMatcher->matches(object.size());
    }

    template<class T>
    void describe(const T& object, Description* description) {
        description->append("container of size ");
        this->sizeMatcher->describe(object.size(), description);
        description->append(": '", object, "'");
    }
private:
    SizeMatcher* sizeMatcher;
};

}

#endif
