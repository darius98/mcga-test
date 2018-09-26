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

    void describeFailure(Description* description) {
        description->append("non-empty iterable");
    }

    void describeSuccess(Description* description) {
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

    void describeFailure(Description* description) {
        description->append("empty iterable");
    }

    void describeSuccess(Description* description) {
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

    void describeSuccess(Description* description) {
        description->append("iterable of size ");
        this->sizeMatcher->describeSuccess(description);
    }

    void describeFailure(Description* description) {
        description->append("iterable of size ");
        this->sizeMatcher->describeFailure(description);
    }


private:
    SizeMatcher* sizeMatcher;
};

}

#endif
