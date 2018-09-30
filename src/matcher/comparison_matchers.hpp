#ifndef RUNTIME_TESTING_MATCHER_COMPARISON_MATCHERS_H_
#define RUNTIME_TESTING_MATCHER_COMPARISON_MATCHERS_H_

#include <functional>
#include <utility>

#include "matcher.hpp"

namespace matcher {

template<class T>
class ComparisonMatcher: public Matcher {
public:
    typedef std::function<bool(const T&, const T&)> Comparator;

    ComparisonMatcher(const T& _target,
                      Comparator _comparator,
                      std::string _expectation) :
            target(_target),
            comparator(_comparator),
            expectation(std::move(_expectation)) {}

    bool matches(const T& object) {
        return comparator(object, target);
    }

    void describe(Description& description) override {
        description << expectation << "'" << target << "'";
    }

    void describeMismatch(Description& description) override {
        description << "not " << expectation << "'" << target << "'";
    }
private:
    Comparator comparator;
    const T& target;
    std::string expectation;
};

} // namespace matcher

#endif
