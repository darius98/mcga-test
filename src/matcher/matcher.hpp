#ifndef RUNTIME_TESTING_MATCHER_MATCHER_H_
#define RUNTIME_TESTING_MATCHER_MATCHER_H_

#include "description.hpp"


namespace matcher {

class BaseMatcher {
public:
    void* operator new(std::size_t size) noexcept;
    void operator delete(void* obj) noexcept;

    static void cleanupMatchersCreatedDuringTests();
private:
    static std::set<void*> matchersAllocatedDuringTests;
};

/// Basic matcher interface.
///
/// The `matches` method is the core method, called with the `expect` top-level
/// function of the library, while `describe` is only called for failed tests.
/// This interface is not extended by all matchers in the library, some matchers
/// preferring to follow the interface where both `matches` and `describe` are
/// template methods, in which case it is not possible to make a pure-virtual
/// interface.
template<class T>
class Matcher: public BaseMatcher {
public:
    /// Check if `object` matches this matcher.
    virtual bool matches(const T& object) = 0;

    /// Describe the way `object` does not match.
    virtual void describe(const T& object, Description* description) = 0;
};

} // namespace matcher

/// Macro for checking if template argument is a matcher-like type.
#define IS_MATCHER(cls)                                                        \
    class=typename std::enable_if<                                             \
            std::is_base_of<matcher::BaseMatcher, cls>::value, void>::type

#endif
