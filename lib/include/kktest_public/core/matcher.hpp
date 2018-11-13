#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MATCHER_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MATCHER_H_

#include <sstream>
#include <string>

#include "./streamer.hpp"

namespace kktest {

class Description {
public:
    template<class T>
    Description& operator<<(T obj) {
        utils::Streamer<T>::send(stream, obj);
        return *this;
    }

    template<class T>
    Description& appendType() {
        utils::Streamer<T>::sendType(stream);
        return *this;
    }

    std::string toString() const {
        return stream.str();
    }
private:
    std::stringstream stream;
};

class Matcher {
public:
    // virtual bool matches(const T& object) = 0;

    virtual void describe(Description& description) = 0;

    virtual void describeMismatch(Description& description) = 0;
};

template<class T, class>
struct is_matcher {
};

template<class T>
struct is_matcher<T, typename std::enable_if<std::is_base_of<Matcher, T>::value, void>::type> {
    typedef T type;
};

}

#define IS_MATCHER(cls) class=typename kktest::is_matcher<cls, void>::type

#endif
