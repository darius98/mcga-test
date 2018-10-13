#ifndef KKTEST_MATCHER_COMPARISON_MATCHERS_H_
#define KKTEST_MATCHER_COMPARISON_MATCHERS_H_

#include <functional>

#include "matcher.hpp"


namespace kktest {

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
    T target;
    std::string expectation;
};

template<class T>
class IdentityMatcher: public Matcher {
public:
    explicit IdentityMatcher(const T& target): address((void*)&target) {}

    bool matches(const T& object) {
        objectAddress = (void*)&object;
        return objectAddress == address;
    }

    void describe(Description& description) override {
        description << "variable at address '" << address << "'";
    }

    void describeMismatch(Description& description) override {
        description << "variable at address '" << objectAddress << "'";
    }

private:
    void* address;
    void* objectAddress = nullptr;
};

} // namespace kktest

#endif
