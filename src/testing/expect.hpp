#ifndef RUNTIME_TESTING_TESTING_EXPECT_H_
#define RUNTIME_TESTING_TESTING_EXPECT_H_

#include <stdexcept>

#include "../matcher/matcher.hpp"


namespace testing {

class ExpectationFailed: std::runtime_error {
public:
    explicit ExpectationFailed(const std::string& str):
            std::runtime_error(str) {}
};

template<class T>
void expect(const T& object, matcher::Matcher<T>* matcher) {
    if (matcher->matches(object)) {
        return;
    }
    auto description = new matcher::Description();
    description->append("expected ");
    matcher->describe(object, description);
    std::string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}

} // namespace testing

#endif
