#ifndef KKTEST_EXT_CORE_MATCHERS_IMPL_COMPARISON_MATCHERS_H_
#define KKTEST_EXT_CORE_MATCHERS_IMPL_COMPARISON_MATCHERS_H_

#include <functional>

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {

template<class T>
class ComparisonMatcher: public Matcher {
public:
    typedef std::function<bool(const T&, const T&)> Comparator;

    ComparisonMatcher(const T& _target, Comparator _comparator, std::string _expectation):
            target(_target), comparator(_comparator), expectation(std::move(_expectation)) {}
    ComparisonMatcher(const ComparisonMatcher& other):
            target(other.target), comparator(other.comparator), expectation(other.expectation) {}

    bool matches(const T& object) {
        return comparator(object, target);
    }

    void describe(Description& description) override {
        description << expectation << "'" << target << "'";
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
    IdentityMatcher(const IdentityMatcher& other): address(other.address) {}

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

template<class T>
ComparisonMatcher<T> isEqualTo(const T& object) {
    return ComparisonMatcher<T>(object, std::equal_to<T>(), "");
}

template<class T>
ComparisonMatcher<T> isNotEqualTo(const T& object) {
    return ComparisonMatcher<T>(object, std::not_equal_to<T>(), "different than ");
}

template<class T>
ComparisonMatcher<T> isLessThan(const T& object) {
    return ComparisonMatcher<T>(object, std::less<T>(), "< ");
}

template<class T>
ComparisonMatcher<T> isLessThanEqual(const T& object) {
    return ComparisonMatcher<T>(object, std::less_equal<T>(), "<= ");
}

template<class T>
ComparisonMatcher<T> isGreaterThan(const T& object) {
    return ComparisonMatcher<T>(object, std::greater<T>(), "> ");
}

template<class T>
ComparisonMatcher<T> isGreaterThanEqual(const T& object) {
    return ComparisonMatcher<T>(object, std::greater_equal<T>(), ">= ");
}

template<class T>
IdentityMatcher<T> isIdenticalTo(const T& object) {
    return IdentityMatcher<T>(object);
}

}

#endif
