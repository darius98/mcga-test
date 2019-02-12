#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_HPP_

#include <kktest_common/string.hpp>
#include <kktest_impl/definers.hpp>
#include <kktest_impl/executable.hpp>
#include <kktest_ext/core_matchers_impl/streamer.hpp>

namespace kktest {
namespace core_matchers {

class Description {
 public:
    Description();
    Description(const Description& other);

    template<class T>
    Description& operator<<(T obj) {
        Streamer<T>::send(stream, obj);
        return *this;
    }

    template<class T>
    Description& appendType() {
        Streamer<T>::sendType(stream);
        return *this;
    }

    Description& appendRawString(const String& str);

    String toString() const;

 private:
    std::stringstream stream;
};

class Matcher {
    // template<class T>
    // virtual bool matches(const T& object) = 0;

    // virtual void describe(Description* description) = 0;

    // ** Will only be called with the same type as `matches` was called. **
    //
    // template<class T>
    // virtual void describeMismatch(Description* description,
    //                               const T& object) = 0;
};

// A little bit of SFINAE never killed anyone :D
namespace detail {

template<class M, class S>
class DescribeMismatchMethodExists {
private:
    template<class T, T> class Check;

    typedef char Yes;
    typedef struct { char _[2]; } No;

    template <class T> struct DescribeMismatchSig {
        typedef void (T::*describeMismatch)(Description*, const S&);
    };

    template <class T> static Yes exists(
            Check<typename DescribeMismatchSig<T>::describeMismatch,
                    &T::describeMismatch>*);
    template <class T> static No  exists(...);

public:
    static constexpr bool value = (sizeof(exists<M>(nullptr)) == sizeof(Yes));
};

template<class M, class S>
class DescribeObjectMethodExists {
private:
    template<class T, T> class Check;

    typedef char Yes;
    typedef struct { char _[2]; } No;

    template <class T> struct DescribeObjectSig {
        typedef void (T::*describeObject)(Description*, const S&);
    };

    template <class T> static Yes exists(
            Check<typename DescribeObjectSig<T>::describeMismatch,
                    &T::describeObject>*);
    template <class T> static No  exists(...);

public:
    static constexpr bool value = (sizeof(exists<M>(nullptr)) == sizeof(Yes));
};

template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<core_matchers::Matcher, M>::value
                >,
        class = typename std::enable_if<
                    DescribeMismatchMethodExists<M, T>::value
                >::type
        >
void __describeMismatch(M& matcher, Description* description, const T& object) {
    matcher.describeMismatch(description, object);
}


template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<core_matchers::Matcher, M>::value
                    && !DescribeMismatchMethodExists<M, T>::value
                >::type
        >
void __describeMismatch(M& matcher, Description* description, const T& object) {
    (*description) << "not ";
    matcher.describe(description);
}


template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<core_matchers::Matcher, M>::value
                >,
        class = typename std::enable_if<
                    DescribeObjectMethodExists<M, T>::value
                >::type
        >
void __describeObject(M& matcher, Description* description, const T& object) {
    matcher.describeObject(description, object);
}

template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<core_matchers::Matcher, M>::value
                    && !DescribeObjectMethodExists<M, T>::value
                >::type
        >
void __describeObject(M& matcher, Description* description, const T& object) {
    (*description) << object;
}

}

}

template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<core_matchers::Matcher, M>::value
                >
        >
void expect(const T& object, M matcher) {
    if (matcher.matches(object)) {
        return;
    }
    core_matchers::Description description;
    description << "Expected ";
    matcher.describe(&description);
    description << "\n\tGot      '" << object << "'\n\tWhich is ";
    core_matchers::detail::__describeMismatch(matcher, &description, object);
    fail("Expectation failed:\n\t" + description.toString());
}

}

#endif
