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
        stream << obj;
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

//    template<class T>
//    virtual bool matches(const T& obj, S* state) const = 0;
//
//    virtual void describe(Description* description) const = 0;
//
//    virtual void describeFailure(Description* description, S* state) const = 0;
};

class StatelessMatcher : public Matcher {
 public:
    static constexpr bool HasState = false;

    typedef int State; // for easier usage of nested matchers.

//    template<class T>
//    virtual bool matches(const T& obj) const = 0;
//
//    virtual void describe(Description* description) const = 0;
//
//    virtual void describeFailure(Description* description) const = 0;
};

namespace detail {

template<class T, class M>
bool __matches(const M& matcher, typename M::State* state, const T& obj) {
    if constexpr (M::HasState) {
        return matcher.matches(obj, state);
    } else {
        return matcher.matches(obj);
    }
}

template<class M>
void __describeFailure(Description* description,
                       const M& matcher,
                       typename M::State* state) {
    if constexpr (M::HasState) {
        matcher.describeFailure(description, state);
    } else {
        matcher.describeFailure(description);
    }
}

}

template<class T, class M>
void expect(const T& obj, M matcher) {
    static_assert(std::is_base_of_v<matchers::Matcher, M>);
    typename M::State state;
    if (detail::__matches(matcher, &state, obj)) {
        return;
    }
    Description description;
    description << "Expected ";
    matcher.describe(&description);
    description << "\n\tGot      '";
    description << obj;
    description << "'\n\tWhich is ";
    detail::__describeFailure(&description, matcher, &state);
    fail("Expectation failed:\n\t" + description.toString());
}

}
