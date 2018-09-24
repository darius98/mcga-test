#ifndef RUNTIME_TESTING_MATCHER_ITERABLE_MATCHER_H_
#define RUNTIME_TESTING_MATCHER_ITERABLE_MATCHER_H_

#include "matcher.hpp"


namespace matcher {

template<class T>
class IsEmptyMatcher: public Matcher<T> {
public:
    bool matches(const T& object) override {
        return object.empty();
    }

    void describe(const T& object, Description* description) override {
        description->append("empty container, got '", object, "'");
    }
};

template<class T>
class SizeMatcher: public Matcher<T> {
public:
    typedef typename T::size_type SizeType;

    explicit SizeMatcher(Matcher<SizeType>* sizeMatcher):
            sizeMatcher(sizeMatcher) {}

    bool matches(const T& object) override {
        this->objectSize = object.size();
        return this->sizeMatcher->matches(this->objectSize);
    }

    void describe(const T& object, Description* description) override {
        description->append("container of size ");
        this->sizeMatcher->describe(this->objectSize, description);
        description->append(": '", object, "'");
    }
private:
    SizeType objectSize = static_cast<SizeType>(-1);
    Matcher<SizeType>* sizeMatcher;
};

}

#endif
