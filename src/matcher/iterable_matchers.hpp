#ifndef RUNTIME_TESTING_MATCHER_ITERABLE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_ITERABLE_MATCHERS_H_

#include "matcher.hpp"

namespace matcher {

template<class ElementMatcher, IS_MATCHER(ElementMatcher)>
class IterableEachMatcher: public BaseMatcher {
public:
    explicit IterableEachMatcher(ElementMatcher* elementMatcher):
            elementMatcher(elementMatcher) {}

    template<class T>
    bool matches(const T& collection) {
        for (const auto& obj: collection) {
            if (!this->elementMatcher->matches(obj)) {
                return false;
            }
        }
        return true;
    }

    template<class T>
    void describe(const T& collection, Description* description) {
        description->append("container where eachElement element is ");
        for (const auto& obj: collection) {
            if (!this->elementMatcher->matches(obj)) {
                this->elementMatcher->describe(obj, description);
                break;
            }
        }
        description->append(": '", collection, "'");
    }
private:
    ElementMatcher* elementMatcher;
};

template<class ElementMatcher, IS_MATCHER(ElementMatcher)>
class IterableAnyMatcher: public BaseMatcher {
public:
    explicit IterableAnyMatcher(ElementMatcher* elementMatcher):
            elementMatcher(elementMatcher) {}

    template<class T>
    bool matches(const T& collection) {
        for (const auto& obj: collection) {
            if (this->elementMatcher->matches(obj)) {
                return true;
            }
        }
        return true;
    }

    template<class T>
    void describe(const T& collection, Description* description) {
        description->append("container where any element is ");
        // TODO(darius98): Find a way to do this!
        description->append(", got '", collection, "'");
    }
private:
    ElementMatcher* elementMatcher;
};

}

#endif
