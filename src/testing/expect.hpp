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

/**
 * Main assertion function.
 */
void __expect(const bool& expr,
              const char* fileName="NO_FILENAME",
              const int& lineNumber=0,
              const std::string& exprStr=nullptr);

/**
 * Function to be used to force a test to fail. `expect` and `expectMatches` are
 * preferred where possible.
 *
 * Has the same effect as __expect(false, fileName, lineNumber, message).
 */
void __fail(const std::string& message,
            const char* fileName="NO_FILENAME",
            const int& lineNumber=0);

/**
 * Main function for expecting something during a test.
 */
template<class T, class PseudoMatcher, IS_MATCHER(PseudoMatcher)>
void __expectMatches(const T& object,
                     PseudoMatcher* matcher,
                     const char* fileName="NO_FILENAME",
                     const int& lineNumber=0) {
    checkDuringTest(fileName, lineNumber);
    if (matcher->matches(object)) {
        return;
    }
    auto description = matcher::Description::createForExpectation(
            fileName,
            lineNumber,
            ""
    );
    matcher->describe(description);
    description->append(". Got '", object, "': ");
    matcher->describeMismatch(description);
    throwExpectationFailed(description);
}

} // namespace runtime_testing

#define expect(expr...)                                                        \
    runtime_testing::__expect(expr, __FILENAME__, __LINE__, #expr " is false")

#define expectMatches(var_comma_matcher...)                                    \
    runtime_testing::__expectMatches(var_comma_matcher, __FILENAME__, __LINE__)

#define fail(...) \
    runtime_testing::__fail(__VA_ARGS__, __FILENAME__, __LINE__)

#endif
