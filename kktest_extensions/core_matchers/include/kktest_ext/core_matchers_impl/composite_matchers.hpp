#ifndef KKTEST_EXT_CORE_MATCHERS_IMPL_COMPOSITE_MATCHERS_H_
#define KKTEST_EXT_CORE_MATCHERS_IMPL_COMPOSITE_MATCHERS_H_

#include <kktest_ext/core_matchers_impl/comparison_matchers.hpp>

namespace kktest {

template<class M1, class M2>
class AndMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M1>::value, "AndMatcher only supports other matchers.");
    static_assert(std::is_base_of<Matcher, M2>::value, "AndMatcher only supports other matchers.");

public:
    AndMatcher(const M1& _m1, const M2& _m2): m1(_m1), m2(_m2) {}

    template<class T>
    bool matches(const T& obj) {
        m1Matches = m1.matches(obj);
        m2Matches = m2.matches(obj);
        return m1Matches && m2Matches;
    }

    void describe(Description& description) override {
        m1.describe(description);
        description << " and ";
        m2.describe(description);
    }

    void describeMismatch(Description& description) override {
        if (m1Matches) {
            m2.describeMismatch(description);
        } else {
            m1.describeMismatch(description);
        }
    }
private:
    M1 m1;
    M2 m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class M1, class M2>
class OrMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M1>::value, "OrMatcher only supports other matchers.");
    static_assert(std::is_base_of<Matcher, M2>::value, "OrMatcher only supports other matchers.");

public:
    OrMatcher(const M1& _m1, const M2& _m2): m1(_m1), m2(_m2) {}

    template<class T>
    bool matches(const T& obj) {
        m1Matches = m1.matches(obj);
        m2Matches = m2.matches(obj);
        return m1Matches || m2Matches;
    }

    void describe(Description& description) override {
        m1.describe(description);
        description << " or ";
        m2.describe(description);
    }

    void describeMismatch(Description& description) override {
        m1.describeMismatch(description);
        description << " and ";
        m2.describeMismatch(description);
    }
private:
    M1 m1;
    M2 m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class M>
class NotMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M>::value, "NotMatcher only supports other matchers.");

public:
    explicit NotMatcher(const M& _matcher): matcher(_matcher) {}

    template<class T>
    bool matches(const T& obj) {
        return !matcher.matches(obj);
    }

    void describe(Description& description) override {
        description << "not ";
        matcher.describe(description);
    }

    void describeMismatch(Description& description) override {
        matcher.describe(description);
    }
private:
    M matcher;
};

template<class A, class B>
auto both(const A& a, const B& b) {
    if constexpr (std::is_base_of<Matcher, A>::value && std::is_base_of<Matcher, B>::value) {
        return AndMatcher<A, B>(a, b);
    } else if constexpr (std::is_base_of<Matcher, A>::value) {
        return AndMatcher<A, EqualityMatcher<B>>(a, isEqualTo(b));
    } else if constexpr (std::is_base_of<Matcher, B>::value) {
        return AndMatcher<EqualityMatcher<A>, B>(isEqualTo(a), b);
    } else {
        return AndMatcher<EqualityMatcher<A>, EqualityMatcher<B>>(isEqualTo(a), isEqualTo(b));
    }
}

template<class A, class B>
auto either(const A& a, const B& b) {
    if constexpr (std::is_base_of<Matcher, A>::value && std::is_base_of<Matcher, B>::value) {
        return OrMatcher<A, B>(a, b);
    } else if constexpr (std::is_base_of<Matcher, A>::value) {
        return OrMatcher<A, EqualityMatcher<B>>(a, isEqualTo(b));
    } else if constexpr (std::is_base_of<Matcher, B>::value) {
        return OrMatcher<EqualityMatcher<A>, B>(isEqualTo(a), b);
    } else {
        return OrMatcher<EqualityMatcher<A>, EqualityMatcher<B>>(isEqualTo(a), isEqualTo(b));
    }
}

template<class T>
auto isNot(const T& x) {
    if constexpr (std::is_base_of<Matcher, T>::value) {
        return NotMatcher<T>(x);
    } else {
        return isNotEqualTo(x);
    }
}

}

#endif
