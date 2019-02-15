#ifndef KKTEST_EXTENSIONS_MATCHERS_KKTEST_EXT_MATCHERS_IMPL_ITERABLE_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_KKTEST_EXT_MATCHERS_IMPL_ITERABLE_MATCHERS_HPP_

#include <kktest_ext/matchers_impl/comparison_matchers.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>

namespace kktest {
namespace matchers {

extern KKTEST_API detail::IsEmptyMatcher isEmpty;

extern KKTEST_API detail::IsNotEmptyMatcher isNotEmpty;

template<
    class T,
    class=typename std::enable_if<std::is_base_of<Matcher, T>::value>::type>
detail::IterableSizeMatcher<T> hasSize(const T& size) {
    return detail::IterableSizeMatcher<T>(size);
}

template<
    class T,
    class=typename std::enable_if<!std::is_base_of<Matcher, T>::value>::type>
detail::IterableSizeMatcher<detail::EqualityMatcher<T>> hasSize(const T& size) {
    return detail::IterableSizeMatcher
            <detail::EqualityMatcher<T>>(isEqualTo(size));
}

template<
    class T,
    class=typename std::enable_if<std::is_base_of<Matcher, T>::value>::type>
detail::IterableEachMatcher<T> eachElement(const T& size) {
    return detail::IterableEachMatcher<T>(size);
}

template<
    class T,
    class=typename std::enable_if<!std::is_base_of<Matcher, T>::value>::type>
detail::IterableEachMatcher<detail::EqualityMatcher<T>> eachElement(
        const T& size) {
    return detail::IterableEachMatcher
            <detail::EqualityMatcher<T>>(isEqualTo(size));
}

template<
    class T,
    class=typename std::enable_if<std::is_base_of<Matcher, T>::value>::type>
detail::IterableAnyMatcher<T> anyElement(const T& size) {
    return detail::IterableAnyMatcher<T>(size);
}

template<
    class T,
    class=typename std::enable_if<!std::is_base_of<Matcher, T>::value>::type>
detail::IterableAnyMatcher<detail::EqualityMatcher<T>> anyElement(
        const T& size) {
    return detail::IterableAnyMatcher
            <detail::EqualityMatcher<T>>(isEqualTo(size));
}

namespace detail {

class IsEmptyMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object.empty();
    }

    KKTEST_API void describe(Description* description);

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "a non-empty iterable";
    }
};

class IsNotEmptyMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return !object.empty();
    }

    KKTEST_API void describe(Description* description);

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "empty iterable";
    }
};

template<class M>
class IterableSizeMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M>::value,
                  "SizeMatcher only supports other matchers as template args.");

 public:
    explicit IterableSizeMatcher(const M& _sizeMatcher):
            sizeMatcher(_sizeMatcher) {}

    template<class T>
    bool matches(const T& object) {
        return sizeMatcher.matches(object.size());
    }

    void describe(Description* description) {
        (*description) << "iterable where size is ";
        sizeMatcher.describe(description);
    }

    template<class T>
    void describeMismatch(Description* description, const T& obj) {
        (*description) << "iterable where size is ";
        sizeMatcher.describeMismatch(description, obj);
    }

 private:
    M sizeMatcher;
};

template<class M>
class IterableEachMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M>::value,
                  "EachMatcher only supports other matchers as template args.");

 public:
    explicit IterableEachMatcher(M _elementMatcher):
            elementMatcher(_elementMatcher) {}

    IterableEachMatcher(const IterableEachMatcher& other):
            elementMatcher(other.elementMatcher),
            elementFailureDescription(other.elementFailureDescription),
            index(other.index) {}

    template<class T>
    bool matches(const T& iterable) {
        index = -1;
        for (const auto& obj : iterable) {
            index += 1;
            if (!elementMatcher.matches(obj)) {
                elementMatcher.describeMismatch(&elementFailureDescription,
                                                obj);
                return false;
            }
        }
        return true;
    }

    void describe(Description* description) {
        (*description) << "an iterable where each element is ";
        elementMatcher.describe(description);
    }

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "an iterable where at index "
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
    explicit IterableAnyMatcher(M _elementMatcher):
            elementMatcher(_elementMatcher) {}
    IterableAnyMatcher(const IterableAnyMatcher& other):
            elementMatcher(other.elementMatcher) {}

    template<class T>
    bool matches(const T& collection) {
        index = -1;
        for (const auto& obj : collection) {
            index += 1;
            if (elementMatcher.matches(obj)) {
                return true;
            }
        }
        return false;
    }

    void describe(Description* description) {
        (*description) << "an iterable where at least one element is ";
        elementMatcher.describe(description);
    }

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "an iterable where no element is ";
        elementMatcher.describe(description);
    }

 private:
    M elementMatcher;
    int index = -1;
};

}
}
}

#endif
