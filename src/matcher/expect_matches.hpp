#ifndef KKTEST_MATCHER_EXPECT_MATCHES_H_
#define KKTEST_MATCHER_EXPECT_MATCHES_H_

#include <testing/definer.hpp>
#include <utils/filename.hpp>

#include "matcher.hpp"

namespace kktest {

class ExpectMatchesDefiner: public Definer {
public:
    using Definer::Definer;

    template<class T, class M, IS_MATCHER(M)>
    void operator()(const T& object, M* matcher) {
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

/**
 * Matcher expectation macro.
 *
 * Use this to verify an object matches a certain matcher.
 *
 * The syntax is `expectMatches(object, matcher)`.
 */
#define expectMatches(...) kktest::ExpectMatchesDefiner(__FILENAME__, __LINE__)\
                                                       (__VA_ARGS__)

#endif
