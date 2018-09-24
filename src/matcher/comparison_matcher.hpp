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
        return this->comparator(object, this->target);
    }

    void describe(const T& object, Description* description) override {
        description->append(
            this->expectation,
            "'",
            this->target,
            "', got '",
            object,
            "'"
        );
    }

private:
    Comparator comparator;
    const T& target;
    const char* expectation;
};

} // namespace matcher

#endif
