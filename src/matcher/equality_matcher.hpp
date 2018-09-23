#ifndef EQUALITY_TRUTH_MATCHER_H_
#define EQUALITY_TRUTH_MATCHER_H_

#include <functional>

#include "matcher.hpp"
#include "../utils/has_operator.hpp"

namespace testing {
namespace matcher {

template<class T>
class EqualityMatcher : public Matcher<T> {
public:
    typedef std::function<bool(const T&, const T&)> EqualityFuncType;

    EqualityMatcher(const T& target, EqualityFuncType equality) :
            target(target), equality(equality) {}

    bool matches(const T& object) const override {
        return this->equality(object, this->target);
    }

    void describe(const T& object,
                  std::stringstream* description) const override {
        (*description) << "expected '";
        if (canPrintObj::value) {
            (*description) << this->target;
        } else {
            (*description) << "value at " << (void*)(&this->target);
        }
        (*description) << "', got '";
        if (canPrintObj::value) {
            (*description) << object;
        } else {
            (*description) << "value at " << (void*)(&object);
        }
        (*description) << "'";
    }

private:
    EqualityFuncType equality;
    const T &target;

    typedef utils::hasLeftShift<std::stringstream, const T&> canPrintObj;
};

template<class T>
bool defaultEquality(const T& a, const T& b) {
    return a == b;
}

template<class T>
Matcher<T>* equals(const T& object) {
    return new EqualityMatcher<T>(object, defaultEquality<T>);
}

template<class T>
bool identityEquality(const T& a, const T& b) {
    return (void*)(&a) == (void*)(&b);
}

template<class T>
Matcher<T>* isIdentical(const T& object) {
    return new EqualityMatcher<T>(object, identityEquality<T>);
}

} // namespace matcher
} // namespace testing

#endif
