#ifndef TESTING_EXPECT_H_
#define TESTING_EXPECT_H_

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
    auto descriptionStream = new std::stringstream();
    matcher->describe(object, descriptionStream);
    std::string s = descriptionStream->str();
    delete descriptionStream;
    throw ExpectationFailed(s);
}

} // namespace testing

#endif
