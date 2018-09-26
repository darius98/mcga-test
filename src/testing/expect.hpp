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

void throwExpectationFailed(matcher::Description* description);

/// Specialization for matcher::Matcher interface, for faster compile-time on
/// basic matchers.
template<class T>
void _expectMatches(const T& object,
                    matcher::Matcher<T>* matcher,
                    const char* fileName="UNKNOWN_FILE",
                    int lineNumber=0,
                    const char* descriptionSuppression=nullptr) {
    checkDuringTest(fileName, lineNumber);
    if (matcher->matches(object)) {
        return;
    }
    auto description = matcher::Description::createForExpectation(
        fileName,
        lineNumber,
        descriptionSuppression
    );
    if (descriptionSuppression == nullptr) {
        matcher->describe(object, description);
    }
    throwExpectationFailed(description);
}

/// Main function for expecting something during a test.
template<class T, class PseudoMatcher, IS_MATCHER(PseudoMatcher)>
void _expectMatches(const T& object,
                    PseudoMatcher* matcher,
                    const char* fileName="NO_FILENAME",
                    int lineNumber=0,
                    const char* descriptionSuppression=nullptr) {
    checkDuringTest(fileName, lineNumber);
    if (matcher->matches(object)) {
        return;
    }
    auto description = matcher::Description::createForExpectation(
        fileName,
        lineNumber,
        descriptionSuppression
    );
    if (descriptionSuppression == nullptr) {
        matcher->describe(object, description);
    }
    throwExpectationFailed(description);
}

void _fail(const std::string& message,
           const char* fileName,
           const int& lineNumber);

} // namespace runtime_testing

#define expect(...)                                                            \
    runtime_testing::_expectMatches(__VA_ARGS__,                               \
                                    isTrue, __FILENAME__, __LINE__,            \
                                    #__VA_ARGS__ ", which is not true")

#define expectMatches(...) \
    runtime_testing::_expectMatches(__VA_ARGS__, __FILENAME__, __LINE__)

#define fail(...) \
    runtime_testing::_fail(__VA_ARGS__, __FILENAME__, __LINE__)

#endif
