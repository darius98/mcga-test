#ifndef KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_COMPARISON_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_CORE_MATCHERS_KKTEST_EXT_CORE_MATCHERS_IMPL_COMPARISON_MATCHERS_HPP_

#include <functional>
#include <string>

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

    void describe(Description* description) {
        (*description) << "'" << target << "'";
    }

 private:
    T target;
};

template<class T>
class NonEqualityMatcher: public Matcher {
 public:
    explicit NonEqualityMatcher(const T& _target): target(_target) {}

    template<class O>
    bool matches(const O& obj) {
        return obj != target;
    }

    void describe(Description* description) {
        (*description) << "not '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "'" << target << "'";
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

    void describe(Description* description) {
        (*description) << "< '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << ">= '" << target << "'";
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

    void describe(Description* description) {
        (*description) << "<= '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "> '" << target << "'";
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

    void describe(Description* description) {
        (*description) << "> '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "<= '" << target << "'";
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

    void describe(Description* description) {
        (*description) << ">= '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "< '" << target << "'";
    }

 private:
    T target;
};

class IdentityMatcher: public Matcher {
 public:
    template<class T>
    explicit IdentityMatcher(const T& target):
            address(static_cast<const void*>(&target)) {}

    IdentityMatcher(const IdentityMatcher& other): address(other.address) {}

    template<class T>
    bool matches(const T& object) {
        return &object == address;
    }

    void describe(Description* description) {
        (*description) << "variable at address '" << address << "'";
    }

    template<class T>
    void describeMismatch(Description* description, const T& object) {
        (*description) << "variable at address '" << &object << "'";
    }

 private:
    const void* address;
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
IdentityMatcher isIdenticalTo(const T& object) {
    return IdentityMatcher(object);
}

}
}

#include "comparison_matchers_string_spec.hpp"

#endif
