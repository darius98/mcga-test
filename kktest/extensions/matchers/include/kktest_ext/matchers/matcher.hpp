#pragma once

#include <any>

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


class Matcher {};

template<class S>
class StatefulMatcher : public Matcher {
 public:
    static constexpr bool HasState = true;

    typedef S State;

    static_assert(std::is_default_constructible_v<State>);

    // template<class T>
    // virtual bool matches(const T& obj, S* state) const = 0;

    virtual void describe(Description* description) const = 0;

    virtual void describeFailure(Description* description, S* state) const = 0;
};

class StatelessMatcher : public Matcher {
 public:
    static constexpr bool HasState = false;

    typedef int State; // for easier usage of nested matchers.

    // template<class T>
    // virtual bool matches(const T& obj) const = 0;

    template<class T>
    inline bool matches(const T& obj, State*) {
        return matches(obj);
    }

    virtual void describe(Description* description) const = 0;

    virtual void describeFailure(Description* description) const = 0;

    inline void describeFailure(Description* description, State*) {
        describeFailure(description);
    }
};

template<
        class T,
        class M,
        class = typename std::enable_if<
                std::is_base_of<matchers::Matcher, M>::value
        >
>
void expect(const T& object, M matcher) {
    if constexpr (M::HasState) {
        typename M::State state;
        if (matcher.matches(object, &state)) {
            return;
        }
        Description description;
        description << "Expected ";
        matcher.describe(&description);
        description << "\n\tGot      '";
        description << object;
        description << "'\n";
        matcher.describeFailure(&description, &state);
        fail("Expectation failed:\n\t" + description.toString());
    } else {
        if (matcher.matches(object)) {
            return;
        }
        Description description;
        description << "Expected ";
        matcher.describe(&description);
        description << "\n\tGot      '";
        description << object;
        description << "'\n";
        matcher.describeFailure(&description);
        fail("Expectation failed:\n\t" + description.toString());
    }
}

}
