#ifndef KTEST_TESTING_EXPECT_H_
#define KTEST_TESTING_EXPECT_H_

#include <stdexcept>

#include "../matcher/matcher.hpp"
#include "testing.hpp"


namespace ktest {

class ExpectationFailed: std::runtime_error {
public:
    explicit ExpectationFailed(const std::string& str);
    ExpectationFailed(const ExpectationFailed& other) noexcept;

    std::string getMessage() const;
};

void checkDuringTest(const std::string& file, int line);

void throwExpectationFailed(Description* description);

void __expect(const bool& exprResult,
              const std::string& file="NO_FILENAME",
              int line=0,
              const std::string& expr="");

template<class T, class M, IS_MATCHER(M)>
void __expectMatches(const T& object,
                     M* matcher,
                     const std::string& file="NO_FILENAME",
                     int line=0) {
    checkDuringTest(file, line);
    if (matcher->matches(object)) {
        return;
    }
    auto description = Description::createForExpect(file, line, "");
    matcher->describe(*description);
    (*description) << ". Got '" << object << "': ";
    matcher->describeMismatch(*description);
    throwExpectationFailed(description);
}

} // namespace ktest

#endif
