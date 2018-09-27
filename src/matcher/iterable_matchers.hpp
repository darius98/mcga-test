#ifndef RUNTIME_TESTING_MATCHER_ITERABLE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_ITERABLE_MATCHERS_H_

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
};

class IsNotEmptyMatcher: public BaseMatcher {
public:
    template<class T>
    bool matches(const T& object) {
        return !object.empty();
    }

    void describeExpectation(Description* description) override;

    void describeFailure(Description* description) override;
};

template<class SizeMatcher, IS_MATCHER(SizeMatcher)>
class IterableSizeMatcher: public BaseMatcher {
public:
    explicit IterableSizeMatcher(SizeMatcher* sizeMatcher):
            sizeMatcher(sizeMatcher) {}

    template<class T>
    bool matches(const T& object) {
        return sizeMatcher->matches(object.size());
    }

    void describeExpectation(Description* description) override {
        description->append("iterable of size ");
        sizeMatcher->describeExpectation(description);
    }

    void describeFailure(Description* description) override {
        description->append("iterable of size ");
        sizeMatcher->describeFailure(description);
    }
private:
    SizeMatcher* sizeMatcher;
};

template<class ElementMatcher, IS_MATCHER(ElementMatcher)>
class IterableEachMatcher: public BaseMatcher {
public:
    explicit IterableEachMatcher(ElementMatcher* elementMatcher):
            elementMatcher(elementMatcher) {}

    template<class T>
    bool matches(const T& iterable) {
        index = -1;
        for (const auto& obj: iterable) {
            index += 1;
            if (!elementMatcher->matches(obj)) {
                elementMatcher->describeFailure(&elementFailureDescription);
                return false;
            }
        }
        return true;
    }

    void describeExpectation(Description* description) override {
        description->append("an iterable where each element is ");
        elementMatcher->describeExpectation(description);
    }

    void describeFailure(Description* description) override {
        description->append(
            "an iterable where at index ", index, " the element is ",
            elementFailureDescription.toString()
        );
    }

private:
    ElementMatcher* elementMatcher;
    Description elementFailureDescription;
    int index = -1;
};

template<class ElementMatcher, IS_MATCHER(ElementMatcher)>
class IterableAnyMatcher: public BaseMatcher {
public:
    explicit IterableAnyMatcher(ElementMatcher* elementMatcher):
            elementMatcher(elementMatcher) {}

    template<class T>
    bool matches(const T& collection) {
        index = -1;
        for (const auto& obj: collection) {
            index += 1;
            if (elementMatcher->matches(obj)) {
                return true;
            }
        }
        return false;
    }

    void describeExpectation(Description* description) override {
        description->append("an iterable where at least one element is ");
        elementMatcher->describeExpectation(description);
    }

    void describeFailure(Description* description) override {
        description->append("an iterable where no element is ");
        elementMatcher->describeExpectation(description);
    }
private:
    ElementMatcher* elementMatcher;
    int index = -1;
};

}

#endif
