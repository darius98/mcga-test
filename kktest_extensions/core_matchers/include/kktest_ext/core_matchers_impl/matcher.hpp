#ifndef KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_H_
#define KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_H_

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

    virtual void describeMismatch(Description& description) {
        description << "not ";
        describe(description);
    }

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
}

#endif
