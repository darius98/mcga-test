#pragma once

#include <kktest.hpp>
#include <kktest_ext/matchers/detail/streamer.hpp>

namespace kktest::matchers {

class Description {
 public:
    Description() = default;
    Description(const Description& other): stream(other.stream.str()) {}

    template<class T>
    Description& operator<<(const T& obj) {
        detail::Streamer<T>::Send(stream, obj);
        return *this;
    }

    template<class T>
    Description& operator<<(const T* obj) {
        detail::Streamer<T>::Send(stream, obj);
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

template<
        class T,
        class M,
        class = typename std::enable_if<
                std::is_base_of<matchers::Matcher, M>::value
        >
>
void expect(const T& object, M matcher) {
    if (matcher.matches(object)) {
        return;
    }
    matchers::Description description;
    description << "Expected ";
    matcher.describe(&description);
    description << "\n\tGot      '";
    description << object;
    description << "'\n";
    matcher.describeMismatch(&description, object);
    fail("Expectation failed:\n\t" + description.toString());
}

}
