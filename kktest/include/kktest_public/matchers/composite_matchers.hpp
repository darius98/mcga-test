#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_COMPOSITE_MATCHERS_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_MATCHERS_COMPOSITE_MATCHERS_H_

#include <kktest_public/matchers/comparison_matchers.hpp>

namespace kktest {

template<class M1, class M2>
class AndMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M1>::value, "AndMatcher only supports other matchers.");
    static_assert(std::is_base_of<Matcher, M2>::value, "AndMatcher only supports other matchers.");

public:
    AndMatcher(M1 _m1, M2 _m2): m1(_m1), m2(_m2) {}

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
    OrMatcher(M1 _m1, M2 _m2): m1(_m1), m2(_m2) {}

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
    explicit NotMatcher(M _matcher): matcher(_matcher) {}

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

template<class M1, class M2>
auto both(M1 m1, M2 m2) {
    if constexpr (std::is_base_of<Matcher, M1>::value && std::is_base_of<Matcher, M2>::value) {
        return AndMatcher<M1, M2>(m1, m2);
    } else if constexpr (std::is_base_of<Matcher, M1>::value) {
        return AndMatcher<M1, ComparisonMatcher<M2>>(m1, isEqualTo(m2));
    } else if constexpr (std::is_base_of<Matcher, M2>::value) {
        return AndMatcher<ComparisonMatcher<M1>, M2>(isEqualTo(m1), m2);
    } else {
        return AndMatcher<ComparisonMatcher<M1>,ComparisonMatcher<M2>>(
                isEqualTo(m1), isEqualTo(m2));
    }
}

template<class M1, class M2>
auto either(M1 m1, M2 m2) {
    if constexpr (std::is_base_of<Matcher, M1>::value && std::is_base_of<Matcher, M2>::value) {
        return OrMatcher<M1, M2>(m1, m2);
    } else if constexpr (std::is_base_of<Matcher, M1>::value) {
        return OrMatcher<M1, ComparisonMatcher<M2>>(m1, isEqualTo(m2));
    } else if constexpr (std::is_base_of<Matcher, M2>::value) {
        return OrMatcher<ComparisonMatcher<M1>, M2>(isEqualTo(m1), m2);
    } else {
        return OrMatcher<ComparisonMatcher<M1>,ComparisonMatcher<M2>>(
                isEqualTo(m1), isEqualTo(m2));
    }
}

template<class M>
auto isNot(M matcher) {
    if constexpr (std::is_base_of<Matcher, M>::value) {
        return NotMatcher<M>(matcher);
    } else {
        return isNotEqualTo(matcher);
    }
}

}

#endif
