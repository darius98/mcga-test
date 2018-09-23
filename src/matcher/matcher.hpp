#ifndef MATCHER_MATCHER_H_
#define MATCHER_MATCHER_H_

#include <sstream>

namespace testing {
namespace matcher {

/// Basic interface that all matchers in this library implement.
///
/// The `matches` method is the core method, called with the `expect` top-level
/// function of the library, while
template<class T>
class Matcher {
public:
    /// Check if `object` matches this matcher.
    virtual bool matches(const T &object) const = 0;

    /// Describe the way `object` does not match.
    virtual void describe(const T &object,
                          std::stringstream *description) const = 0;
};

} // namespace matcher
} // namespace testing

#endif
