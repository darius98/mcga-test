#ifndef RUNTIME_TESTING_TESTING_EXPECT_H_
#define RUNTIME_TESTING_TESTING_EXPECT_H_

#include <stdexcept>

#include "../matcher/matcher.hpp"
#include "testing.hpp"


#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ?                       \
                      __builtin_strrchr(__FILE__, '/') + 1 :                   \
                      __FILE__)


namespace runtime_testing {

class ExpectationFailed: std::runtime_error {
public:
    explicit ExpectationFailed(const std::string& str);
    ExpectationFailed(const ExpectationFailed& other) noexcept;

    std::string getMessage() const;
};

void checkDuringTest(const char* fileName, const int& lineNumber);

void throwExpectationFailed(
        matcher::Description* description,
        const char* fileName,
        const int& lineNumber);

/// Specialization for matcher::Matcher interface, for faster compile-time on
/// basic matchers.
template<class T>
void _expectMatches(const T& object,
                    matcher::Matcher<T>* matcher,
                    const char* fileName="UNKNOWN_FILE",
                    int lineNumber=0) {
    checkDuringTest(fileName, lineNumber);
    if (matcher->matches(object)) {
        return;
    }
    auto description = matcher::Description::createForExpectation();
    matcher->describe(object, description);
    throwExpectationFailed(description, fileName, lineNumber);
}

/// Main function for expecting something during a test.
template<class T, class PseudoMatcher>
void _expectMatches(const T& object,
                    PseudoMatcher* matcher,
                    const char* fileName = "NO_FILENAME",
                    int lineNumber = 0) {
    checkDuringTest(fileName, lineNumber);
    if (matcher->matches(object)) {
        return;
    }
    auto description = matcher::Description::createForExpectation();
    matcher->describe(object, description);
    throwExpectationFailed(description, fileName, lineNumber);
}

#define expect(expr) \
    runtime_testing::_expectMatches(expr, isTrue, __FILENAME__, __LINE__)

#define expectMatches(expr, matcher) \
    runtime_testing::_expectMatches(expr, matcher, __FILENAME__, __LINE__)

} // namespace runtime_testing

#endif
