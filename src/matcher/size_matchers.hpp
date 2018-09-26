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

    void describeExpectation(Description* description) override;

    void describeFailure(Description* description) override;

    void describeSuccess(Description* description) override;
};

class IsNotEmptyMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return !object.empty();
    }

    void describeExpectation(Description* description) override;

    void describeFailure(Description* description) override;

    void describeSuccess(Description* description) override;
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

    void describeExpectation(Description* description) override {
        description->append("iterable of size ");
        this->sizeMatcher->describeExpectation(description);
    }

    void describeSuccess(Description* description) override {
        description->append("iterable of size ");
        this->sizeMatcher->describeSuccess(description);
    }

    void describeFailure(Description* description) override {
        description->append("iterable of size ");
        this->sizeMatcher->describeFailure(description);
    }


private:
    SizeMatcher* sizeMatcher;
};

}

#endif
