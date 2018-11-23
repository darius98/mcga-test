#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_MATCHER_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_MATCHER_H_

#include "./streamer.hpp"

namespace kktest {

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

    template<class T>
    std::string buildMismatchMessage(const T& object) {
        Description description;
        description << "Expected ";
        describe(description);
        description << ".\n\tGot '" << object << "'.\n\tWhich is ";
        describeMismatch(description);
        description << ".";
        return description.toString();
    }
};

}

#endif
