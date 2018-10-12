#ifndef KKTEST_MATCHER_EXPECT_MATCHES_H_
#define KKTEST_MATCHER_EXPECT_MATCHES_H_

#include <testing/definer.hpp>
#include <utils/filename.hpp>

#include "matcher.hpp"

namespace kktest {

class ExpectMatchesDefiner: public ExpectDefiner {
public:
    using ExpectDefiner::ExpectDefiner;

    void operator()(const bool& result, const std::string& expr) override;

    template<class T, class M, class... Args, IS_MATCHER(M)>
    void operator()(const T& object, M* matcher, const Args... args) {
        checkDuringTest();
        if (matcher->matches(object)) {
            return;
        }
        auto description = Description::createForExpect(file, line, "");
        matcher->describe(*description);
        (*description) << ". Got '" << object << "': ";
        matcher->describeMismatch(*description);
        throwExpectationFailed(description);
    }

private:
    void throwExpectationFailed(Description* description);

    void checkDuringTest();
};

}

#ifdef EXPECT_DEFINED
#undef expect
#undef fail
#endif

/**
 * Matcher expectation macro.
 *
 * Use this to verify an object matches a certain matcher.
 *
 * The syntax is `expect(object, matcher)`.
 */
#define expect(...) kktest::ExpectMatchesDefiner(__FILENAME__, __LINE__)       \
                                         (__VA_ARGS__, #__VA_ARGS__ " is false")

#define fail(...)   kktest::ExpectMatchesDefiner(__FILENAME__, __LINE__)       \
                                         (false, __VA_ARGS__)

#define EXPECT_DEFINED

#endif
