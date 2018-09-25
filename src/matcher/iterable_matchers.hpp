#ifndef RUNTIME_TESTING_MATCHER_ITERABLE_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_ITERABLE_MATCHERS_H_

#include "matcher.hpp"

namespace matcher {

template<class EachMatcher>
class CollectionEachMatcher {
public:
    explicit CollectionEachMatcher(EachMatcher* eachMatcher):
            eachMatcher(eachMatcher) {}

    template<class T>
    bool matches(const T& collection) {
        for (const auto& obj: collection) {
            if (!this->eachMatcher->matches(obj)) {
                return false;
            }
        }
        return true;
    }

    template<class T>
    void describe(const T& collection, Description* description) {
        description->append("container where eachElement element is ");
        for (const auto& obj: collection) {
            if (!this->eachMatcher->matches(obj)) {
                this->eachMatcher->describe(obj, description);
                break;
            }
        }
        description->append(": '", collection, "'");
    }
private:
    EachMatcher* eachMatcher;
};

}

#endif
