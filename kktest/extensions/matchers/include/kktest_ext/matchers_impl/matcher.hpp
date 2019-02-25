#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_MATCHER_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_MATCHER_HPP_

#include <kktest.hpp>
#include <kktest_ext/matchers_impl/detail/streamer.hpp>

namespace kktest {
namespace matchers {

class Description {
 public:
    Description() = default;
    Description(const Description& other): stream(other.stream.str()) {}

    template<class T>
    Description& operator<<(const T& obj) {
        detail::Streamer<T>::send(stream, obj);
        return *this;
    }

    template<class T>
    Description& operator<<(const T* obj) {
        detail::Streamer<T>::send(stream, obj);
        return *this;
    }

    Description& appendRawString(const std::string& str) {
        stream << str;
        return *this;
    }

    std::string toString() const {
        return stream.str();
    }

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

}
}

#endif