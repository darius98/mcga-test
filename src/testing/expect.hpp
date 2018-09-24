#ifndef RUNTIME_TESTING_TESTING_EXPECT_H_
#define RUNTIME_TESTING_TESTING_EXPECT_H_

#include <stdexcept>

#include "../matcher/matcher.hpp"
#include "test.hpp"


#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ?                       \
                      __builtin_strrchr(__FILE__, '/') + 1 :                   \
                      __FILE__)


namespace runtime_testing {

class ExpectationFailed: std::runtime_error {
public:
    explicit ExpectationFailed(const std::string& str):
            std::runtime_error(str) {}
    ExpectationFailed(const ExpectationFailed& other) noexcept:
            ExpectationFailed(other.getMessage()) {}

    std::string getMessage() const { return this->what(); }
};

/// Main function for expecting something during a test.
template<class T, class PseudoMatcher>
void _expectMatches(const T& object,
                    PseudoMatcher* matcher,
                    const char* filename = "NO_FILENAME",
                    int lineNumber = 0) {
    if (!isDuringTest()) {
        throw std::runtime_error(
                std::string("'expect' can only be called inside tests! ") +
                "(" + filename + ":" + std::to_string(lineNumber) + ")"
        );
    }
    if (matcher->matches(object)) {
        return;
    }
    auto description = new matcher::Description();
    description->append("expected ");
    matcher->describe(object, description);
    description->append(
        std::string(" (") + filename + ":" + std::to_string(lineNumber) + ")"
    );
    std::string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}

#define expect(expr) \
    runtime_testing::_expectMatches(expr, isTrue, __FILENAME__, __LINE__)

#define expectMatches(expr, matcher) \
    runtime_testing::_expectMatches(expr, matcher, __FILENAME__, __LINE__)

} // namespace runtime_testing

#endif
