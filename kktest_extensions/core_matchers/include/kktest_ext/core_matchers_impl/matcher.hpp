#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_HPP_

#include <kktest_impl/types.hpp>
#include <kktest_ext/core_matchers_impl/streamer.hpp>

namespace kktest {
namespace core_matchers {

class Description {
 public:
    Description() = default;
    Description(const Description& other): stream(other.stream.str()) {}

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

    String toString() const {
        return String(stream.str());
    }
 private:
    std::stringstream stream;
};

class Matcher {
 public:
    // virtual bool matches(const T& object) = 0;

    virtual void describe(Description& description) = 0;

    virtual void describeMismatch(Description& description) {
        description << "not ";
        describe(description);
    }

    template<class T>
    String buildMismatchMessage(const T& object) {
        Description description;
        description << "Expected ";
        describe(description);
        description << ".\n\tGot '" << object << "'.\n\tWhich is ";
        describeMismatch(description);
        description << ".";
        return description.toString();
    }
};

}  // namespace core_matchers
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_CORE_MATCHERS_INCLUDE_KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_HPP_
