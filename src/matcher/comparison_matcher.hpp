#ifndef EQUALITY_TRUTH_MATCHER_H_
#define EQUALITY_TRUTH_MATCHER_H_

#include <functional>
#include <set>

#include "matcher.hpp"
#include "../utils/has_operator.hpp"

namespace matcher {

template<class T>
class ComparisonMatcher : public Matcher<T> {
public:
    typedef std::function<bool(const T&, const T&)> Comparator;

    ComparisonMatcher(const T& target,
                      Comparator comparator,
                      const char* expectation) :
            target(target), comparator(comparator), expectation(expectation) {}

    bool matches(const T& object) const override {
        return this->comparator(object, this->target);
    }

    void describe(const T& object,
                  std::stringstream* description) const override {
        (*description) << this->expectation << " '";
        if (ComparisonMatcher<T>::canPrintObj::value) {
            (*description) << this->target;
        } else {
            (*description) << "value at " << (void*)(&this->target);
        }
        (*description) << "', got '";
        if (ComparisonMatcher<T>::canPrintObj::value) {
            (*description) << object;
        } else {
            (*description) << "value at " << (void*)(&object);
        }
        (*description) << "'";
    }

private:
    Comparator comparator;
    const T &target;
    const char* expectation;

    typedef testing::utils::hasLeftShift<std::stringstream, const T&>
            canPrintObj;
};

} // namespace matcher

#endif
