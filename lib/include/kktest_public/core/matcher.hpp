#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MATCHER_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_MATCHER_H_

#include "./streamer.hpp"

namespace kktest {

class Description {
public:
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

}

#endif
