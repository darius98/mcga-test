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

void checkDuringTest(const char* file, int line);

void throwExpectationFailed(matcher::Description* description);

void __expect(const bool& exprResult,
              const char* file="NO_FILENAME",
              int line=0,
              const std::string& expr);

template<class T, class PseudoMatcher, IS_MATCHER(PseudoMatcher)>
void __expectMatches(const T& object,
                     PseudoMatcher* matcher,
                     const char* file="NO_FILENAME",
                     int line=0) {
    checkDuringTest(file, line);
    if (matcher->matches(object)) {
        return;
    }
    auto description = matcher::Description::createForExpect(file, line, "");
    matcher->describe(description);
    description->append(". Got '", object, "': ");
    matcher->describeMismatch(description);
    throwExpectationFailed(description);
}

} // namespace runtime_testing


/**
 * Assertion macro.
 *
 * Use this to verify a boolean condition is satisfied. Fails the test, printing
 * the body of the boolean expression when it evaluates to false.
 */
#define expect(expr...)                                                        \
    runtime_testing::__expect(expr, __FILENAME__, __LINE__, #expr " is false")

/**
 * Matcher expectation macro.
 *
 * Use this to verify an object matches a certain matcher.
 *
 * The syntax is `expectMatches(object, matcher)`.
 */
#define expectMatches(var_comma_matcher...)                                    \
    runtime_testing::__expectMatches(var_comma_matcher, __FILENAME__, __LINE__)


/**
 * Macro for forcing a test to fail. `expect` and `expectMatches` are
 * preferred where possible.
 *
 * Has the same effect as __expect(false, file, line, message).
 */
#define fail(...) \
    runtime_testing::__expect(false, __FILENAME__, __LINE__, __VA_ARGS__)

#endif
