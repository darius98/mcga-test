#ifndef KTEST_MATCHER_MATCHER_H_
#define KTEST_MATCHER_MATCHER_H_

#include "description.hpp"


namespace ktest {

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
    static void cleanupMatchersCreatedDuringTests();

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
    static std::set<void*> matchersAllocatedDuringTests;
};

} // namespace ktest

/**
 * Macro for checking if template argument is a matcher type.
 */
#define IS_MATCHER(cls)                                                        \
    class=typename std::enable_if<                                             \
            std::is_base_of<ktest::Matcher, cls>::value, void>::type

#endif
