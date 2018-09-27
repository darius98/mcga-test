#ifndef RUNTIME_TESTING_MATCHER_COMPARISON_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_COMPARISON_MATCHERS_H_

#include <functional>

#include "matcher.hpp"

namespace matcher {

template<class T>
class ComparisonMatcher: public Matcher {
public:
    typedef std::function<bool(const T&, const T&)> Comparator;

    ComparisonMatcher(const T& target,
                      Comparator comparator,
                      const char* expectation) :
            target(target), comparator(comparator), expectation(expectation) {}

    bool matches(const T& object) {
        return comparator(object, target);
    }

    void describe(Description* description) override {
        description->append(expectation, "'", target, "'");
    }

    void describeMismatch(Description* description) override {
        description->append("not ", expectation, "'", target, "'");
    }
private:
    Comparator comparator;
    const T& target;
    const char* expectation;
};

} // namespace matcher

#endif
