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

    void describeExpectation(Description* description) {
        description->append("empty iterable");
    }

    template<class T>
    void describeFailure(const T& object, Description* description) {
        description->append("non-empty iterable");
    }

    template<class T>
    void describeSuccess(const T& object, Description* description) {
        description->append("empty iterable");
    }
};

class IsNotEmptyMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return !object.empty();
    }

    void describeExpectation(Description* description) {
        description->append("non-empty iterable");
    }

    template<class T>
    void describeFailure(const T& object, Description* description) {
        description->append("empty iterable");
    }

    template<class T>
    void describeSuccess(const T& object, Description* description) {
        description->append("non-empty iterable");
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

    void describeExpectation(Description* description) {
        description->append("iterable of size ");
        this->sizeMatcher->describeExpectation(description);
    }

    template<class T>
    void describeSuccess(const T& object, Description* description) {
        description->append("iterable of size ");
        this->sizeMatcher->describeSuccess(object.size(), description);
    }

    template<class T>
    void describeFailure(const T& object, Description* description) {
        description->append("iterable of size ");
        this->sizeMatcher->describeFailure(object.size(), description);
    }


private:
    SizeMatcher* sizeMatcher;
};

}

#endif
