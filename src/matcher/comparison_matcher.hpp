#ifndef RUNTIME_TESTING_MATCHER_COMPARISON_MATCHER_H_
#define RUNTIME_TESTING_MATCHER_COMPARISON_MATCHER_H_

#include <functional>

#include "matcher.hpp"

namespace matcher {

template<class T>
class ComparisonMatcher : public Matcher<T> {
public:
    typedef std::function<bool(const T&, const T&)> Comparator;

    ComparisonMatcher(const T& target,
                      Comparator comparator,
                      const char* expectation) :
            target(target), comparator(comparator), expectation(expectation) {}

    bool matches(const T& object) override {
        return comparator(object, target);
    }

    void describeExpectation(Description* description) override {
        description->append(
            "an object that is ", expectation, "'", target, "'"
        );
    }

    void describeFailure(Description* description) override {
        description->append(
            "an object that is not ", expectation, "'", target, "'"
        );
    }
private:
    Comparator comparator;
    const T& target;
    const char* expectation;
};

} // namespace matcher

#endif
