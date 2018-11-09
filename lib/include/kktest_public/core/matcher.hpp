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

    std::string toString() const;
private:
    std::stringstream stream;
};

/**
 * Basic matcher interface.
 *
 * The `matches` method is the core method, called with the `expectMatches`
 * macro of the library, while `describe` and `describeMismatch` are only
 * called for failed tests.
 *
 * This interface is extended by all matchers in the library, although it is
 * each matcher's responsibility to define the `matches` method only for types
 * it wishes to support (we cannot define a pure-virtual template method inside
 * this interface).
 */
class Matcher {
public:
    void* operator new(std::size_t size) noexcept;

    void operator delete(void* obj) noexcept;

    /**
     * Check if an object matches this matcher.
     *
     * This is the core method of the matcher. Choose your own types for which
     * this matcher is intended.
     */
    // virtual bool matches(const T& object) = 0;

    /**
     * Describe what this matcher expects from `object`.
     */
    virtual void describe(Description& description) = 0;

    /**
     * Describe the way `object` does not match expectations.
     */
    virtual void describeMismatch(Description& description) = 0;

private:
    static void cleanupMatchersCreatedDuringTests();

    static bool duringTest;
    static bool matcherPluginEnabled;
    static bool testingStarted;
    static std::set<void*> matchersAllocatedDuringTests;

friend class MatcherPlugin;
};

template<class T, class>
struct is_matcher { };

template<class T>
struct is_matcher<
    T, typename std::enable_if<std::is_base_of<Matcher, T>::value, void>::type>
    { typedef T type; };

} // namespace kktest

/**
 * Macro for checking if template argument is a matcher type.
 */
#define IS_MATCHER(cls) class=typename is_matcher<cls, void>::type

#endif
