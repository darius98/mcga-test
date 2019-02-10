#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_MATCHER_HPP_

#include <kktest_common/string.hpp>
#include <kktest_impl/executable.hpp>
#include <kktest_ext/core_matchers_impl/streamer.hpp>

namespace kktest {
namespace core_matchers {

class Description {
 public:
    Description();
    Description(const Description& other);

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

    Description& appendRawString(const String& str);

    String toString() const;

 private:
    std::stringstream stream;
};

class Matcher {
 public:
    // virtual bool matches(const T& object) = 0;

    virtual void describe(Description* description) = 0;

    virtual void describeMismatch(Description* description);

    template<class T>
    String buildMismatchMessage(const T& object) {
        Description description;
        description << "Expected ";
        describe(&description);
        description << "\n\tGot      '" << object << "'\n\tWhich is ";
        describeMismatch(&description);
        return description.toString();
    }
};

}

template<class T, class M, class=typename std::enable_if<
                               std::is_base_of<core_matchers::Matcher, M>::value
                                                         >::type>
void expect(const T& object, M matcher) {
    if (matcher.matches(object)) {
        return;
    }
    fail("Expectation failed:\n\t" + matcher.buildMismatchMessage(object));
}

}

#endif
