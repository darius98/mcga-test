#ifndef KKTEST_EXTENSIONS_MATCHERS_KKTEST_EXT_MATCHERS_IMPL_MATCHER_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_KKTEST_EXT_MATCHERS_IMPL_MATCHER_HPP_

#include <kktest_common/string.hpp>
#include <kktest_impl/definers.hpp>
#include <kktest_impl/executable.hpp>
#include <kktest_ext/matchers_impl/detail/streamer.hpp>

namespace kktest {
namespace matchers {

class Description {
 public:
    Description();
    Description(const Description& other);

    template<class T>
    Description& operator<<(T obj) {
        detail::Streamer<T>::send(stream, obj);
        return *this;
    }

    template<class T>
    Description& appendType() {
        detail::Streamer<T>::sendType(stream);
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
                    &T::describeMismatch>*) { return 'a'; }
    template <class T> static No  exists(...) { return {'a', 'b'}; }

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
                    &T::describeObject>*) { return 'a'; }
    template <class T> static No  exists(...) { return {'a', 'b'}; }

public:
    static constexpr bool value = (sizeof(exists<M>(nullptr)) == sizeof(Yes));
};

template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<matchers::Matcher, M>::value
                >,
        class = typename std::enable_if<
                    DescribeMismatchMethodExists<M, T>::value
                >::type
        >
void __describeMismatch(M& matcher, Description* description, const T& object) {
    (*description) << "\tWhich is ";
    matcher.describeMismatch(description, object);
}


template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<matchers::Matcher, M>::value
                    && !DescribeMismatchMethodExists<M, T>::value
                >::type
        >
void __describeMismatch(M&, Description*, const T&) {}


template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<matchers::Matcher, M>::value
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
                    std::is_base_of<matchers::Matcher, M>::value
                    && !DescribeObjectMethodExists<M, T>::value
                >::type
        >
void __describeObject(M& matcher, Description* description, const T& object) {
    (*description) << object;
}

}
}
}

#endif
