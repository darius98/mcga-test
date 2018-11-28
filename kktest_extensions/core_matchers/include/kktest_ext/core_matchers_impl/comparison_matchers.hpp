#ifndef KKTEST_EXT_CORE_MATCHERS_IMPL_COMPARISON_MATCHERS_H_
#define KKTEST_EXT_CORE_MATCHERS_IMPL_COMPARISON_MATCHERS_H_

#include <functional>

#include <kktest_ext/core_matchers_impl/matcher.hpp>

namespace kktest {
namespace core_matchers {

template<class T>
class EqualityMatcher: public Matcher {
public:
    explicit EqualityMatcher(const T& _target): target(_target) {}

    template<class O>
    bool matches(const O& object) {
        return object == target;
    }

    void describe(Description& description) override {
        description << "'" << target << "'";
    }

private:
    T target;
};

template<class T>
class NonEqualityMatcher: public Matcher {
public:
    explicit NonEqualityMatcher(const T& _target): target(_target) {}

    template<class O>
    bool matches(const O& object) {
        return object != target;
    }

    void describe(Description& description) override {
        description << "not '" << target << "'";
    }

    void describeMismatch(Description& description) override {
        description << "'" << target << "'";
    }

private:
    T target;
};

template<class T>
class IsLessThanMatcher: public Matcher {
public:
    explicit IsLessThanMatcher(const T& _target): target(_target) {}

    template<class O>
    bool matches(const O& object) {
        return object < target;
    }

    void describe(Description& description) override {
        description << "< '" << target << "'";
    }

    void describeMismatch(Description& description) override {
        description << ">= '" << target << "'";
    }

private:
    T target;
};

template<class T>
class IsLessThanEqualMatcher: public Matcher {
public:
    explicit IsLessThanEqualMatcher(const T& _target): target(_target) {}

    template<class O>
    bool matches(const O& object) {
        return object <= target;
    }

    void describe(Description& description) override {
        description << "<= '" << target << "'";
    }

    void describeMismatch(Description& description) override {
        description << "> '" << target << "'";
    }

private:
    T target;
};

template<class T>
class IsGreaterThanMatcher: public Matcher {
public:
    explicit IsGreaterThanMatcher(const T& _target): target(_target) {}

    template<class O>
    bool matches(const O& object) {
        return object > target;
    }

    void describe(Description& description) override {
        description << "> '" << target << "'";
    }

    void describeMismatch(Description& description) override {
        description << "<= '" << target << "'";
    }

private:
    T target;
};

template<class T>
class IsGreaterThanEqualMatcher: public Matcher {
public:
    explicit IsGreaterThanEqualMatcher(const T& _target): target(_target) {}

    template<class O>
    bool matches(const O& object) {
        return object >= target;
    }

    void describe(Description& description) override {
        description << ">= '" << target << "'";
    }

    void describeMismatch(Description& description) override {
        description << "< '" << target << "'";
    }

private:
    T target;
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
EqualityMatcher<T> isEqualTo(const T& object) {
    return EqualityMatcher<T>(object);
}

template<class T>
NonEqualityMatcher<T> isNotEqualTo(const T& object) {
    return NonEqualityMatcher<T>(object);
}

template<class T>
IsLessThanMatcher<T> isLessThan(const T& object) {
    return IsLessThanMatcher<T>(object);
}

template<class T>
IsLessThanEqualMatcher<T> isLessThanEqual(const T& object) {
    return IsLessThanEqualMatcher<T>(object);
}

template<class T>
IsGreaterThanMatcher<T> isGreaterThan(const T& object) {
    return IsGreaterThanMatcher<T>(object);
}

template<class T>
IsGreaterThanEqualMatcher<T> isGreaterThanEqual(const T& object) {
    return IsGreaterThanEqualMatcher<T>(object);
}

template<class T>
IdentityMatcher<T> isIdenticalTo(const T& object) {
    return IdentityMatcher<T>(object);
}

}
}

#endif
