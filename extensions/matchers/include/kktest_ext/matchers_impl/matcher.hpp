#ifndef KKTEST_EXTENSIONS_MATCHERS_KKTEST_EXT_MATCHERS_IMPL_MATCHER_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_KKTEST_EXT_MATCHERS_IMPL_MATCHER_HPP_

#include <kktest_common/string.hpp>
#include <kktest_impl/definers.hpp>
#include <kktest_impl/executable.hpp>
#include <kktest_ext/matchers_impl/detail/streamer.hpp>

namespace kktest {
namespace matchers {

class KKTEST_API Description {
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

class KKTEST_API Matcher {
    // template<class T>
    // virtual bool matches(const T& object) = 0;

    // virtual void describe(Description* description) = 0;

    // ** Will only be called with the same type as `matches` was called. **
    //
    // template<class T>
    // virtual void describeMismatch(Description* description,
    //                               const T& object) = 0;
};

}
}

#endif
