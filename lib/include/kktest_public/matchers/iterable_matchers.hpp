#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_ITERABLE_MATCHERS_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_ITERABLE_MATCHERS_H_

#include "./comparison_matchers.hpp"


namespace kktest {

class IsEmptyMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return object.empty();
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

class IsNotEmptyMatcher: public Matcher {
public:
    template<class T>
    bool matches(const T& object) {
        return !object.empty();
    }

    void describe(Description& description) override;

    void describeMismatch(Description& description) override;
};

template<class M>
class IterableSizeMatcher: public Matcher {
public:
    explicit IterableSizeMatcher(M _sizeMatcher): sizeMatcher(_sizeMatcher) {}

    template<class T>
    bool matches(const T& object) {
        return sizeMatcher.matches(object.size());
    }

    void describe(Description& description) override {
        description << "iterable where size is ";
        sizeMatcher.describe(description);
    }

    void describeMismatch(Description& description) override {
        description << "iterable where size is ";
        sizeMatcher.describeMismatch(description);
    }
private:
    M sizeMatcher;
};

template<class M>
class IterableEachMatcher: public Matcher {
public:
    explicit IterableEachMatcher(M _elementMatcher): elementMatcher(_elementMatcher) {}

    IterableEachMatcher(const IterableEachMatcher& other):
            elementMatcher(other.elementMatcher),
            elementFailureDescription(other.elementFailureDescription),
            index(other.index) {}

    template<class T>
    bool matches(const T& iterable) {
        index = -1;
        for (const auto& obj: iterable) {
            index += 1;
            if (!elementMatcher.matches(obj)) {
                elementMatcher.describeMismatch(elementFailureDescription);
                return false;
            }
        }
        return true;
    }

    void describe(Description& description) override {
        description << "an iterable where each element is ";
        elementMatcher.describe(description);
    }

    void describeMismatch(Description& description) override {
        description << "an iterable where at index "
                    << index
                    << " the element is "
                    << elementFailureDescription.toString();
    }

private:
    M elementMatcher;
    Description elementFailureDescription;
    int index = -1;
};

template<class M>
class IterableAnyMatcher: public Matcher {
public:
    explicit IterableAnyMatcher(M _elementMatcher): elementMatcher(_elementMatcher) {}
    IterableAnyMatcher(const IterableAnyMatcher& other): elementMatcher(other.elementMatcher) {}

    template<class T>
    bool matches(const T& collection) {
        index = -1;
        for (const auto& obj: collection) {
            index += 1;
            if (elementMatcher.matches(obj)) {
                return true;
            }
        }
        return false;
    }

    void describe(Description& description) override {
        description << "an iterable where at least one element is ";
        elementMatcher.describe(description);
    }

    void describeMismatch(Description& description) override {
        description << "an iterable where no element is ";
        elementMatcher.describe(description);
    }
private:
    M elementMatcher;
    int index = -1;
};

extern IsEmptyMatcher isEmpty;

extern IsNotEmptyMatcher isNotEmpty;

template<class T>
auto hasSize(const T& size) {
    if constexpr (std::is_base_of<Matcher, T>::value) {
        return IterableSizeMatcher<T>(size);
    } else {
        return IterableSizeMatcher<ComparisonMatcher<T>>(isEqualTo(size));
    }
}

template<class T>
auto eachElement(const T& each) {
    if constexpr (std::is_base_of<Matcher, T>::value) {
        return IterableEachMatcher<T>(each);
    } else {
        return IterableEachMatcher<ComparisonMatcher<T>>(isEqualTo(each));
    }
}

template<class T>
auto anyElement(const T& any) {
    if constexpr (std::is_base_of<Matcher, T>::value) {
        return IterableAnyMatcher<T>(any);
    } else {
        return IterableAnyMatcher<ComparisonMatcher<T>>(isEqualTo(any));
    }
}

}

#endif
