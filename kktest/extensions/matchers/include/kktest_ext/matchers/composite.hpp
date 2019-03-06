#pragma once

#include <kktest_ext/matchers/comparison.hpp>
#include <kktest_ext/matchers/detail/decl.hpp>

namespace kktest::matchers {

template<class A, class B>
constexpr auto both(const A& a, const B& b) {
    if constexpr (std::is_base_of_v<Matcher, A>) {
        if constexpr (std::is_base_of_v<Matcher, B>) {
            return detail::AndMatcher(a, b);
        } else {
            return detail::AndMatcher(a, isEqualTo(b));
        }
    } else {
        if constexpr (std::is_base_of_v<Matcher, B>) {
            return detail::AndMatcher(isEqualTo(a), b);
        } else {
            return detail::AndMatcher(isEqualTo(a), isEqualTo(b));
        }
    }
}

template<class A, class B>
constexpr auto either(const A& a, const B& b) {
    if constexpr (std::is_base_of_v<Matcher, A>) {
        if constexpr (std::is_base_of_v<Matcher, B>) {
            return detail::OrMatcher(a, b);
        } else {
            return detail::OrMatcher(a, isEqualTo(b));
        }
    } else {
        if constexpr (std::is_base_of_v<Matcher, B>) {
            return detail::OrMatcher(isEqualTo(a), b);
        } else {
            return detail::OrMatcher(isEqualTo(a), isEqualTo(b));
        }
    }
}

template<class T>
constexpr auto isNot(const T& x) {
    if constexpr (std::is_base_of_v<Matcher, T>) {
        return detail::NotMatcher(x);
    } else {
        return detail::NotMatcher(isEqualTo(x));
    }
}

namespace detail {

template<class M1, class M2>
class AndMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M1>::value,
                  "AndMatcher only supports other matchers as template args.");
    static_assert(std::is_base_of<Matcher, M2>::value,
                  "AndMatcher only supports other matchers as template args.");

 public:
    constexpr AndMatcher(const M1& m1, const M2& m2): m1(m1), m2(m2) {}

    template<class T>
    bool matches(const T& obj) {
        m1Matches = m1.matches(obj);
        m2Matches = m2.matches(obj);
        return m1Matches && m2Matches;
    }

    void describe(Description* description) {
        m1.describe(description);
        (*description) << " and ";
        m2.describe(description);
    }

    template<class T>
    void describeMismatch(Description* description, const T& obj) {
        if (m1Matches) {
            m2.describeMismatch(description, obj);
        } else {
            m1.describeMismatch(description, obj);
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
    static_assert(std::is_base_of<Matcher, M1>::value,
                  "OrMatcher only supports other matchers as template args.");
    static_assert(std::is_base_of<Matcher, M2>::value,
                  "OrMatcher only supports other matchers as template args.");

 public:
    constexpr OrMatcher(const M1& m1, const M2& m2): m1(m1), m2(m2) {}

    template<class T>
    bool matches(const T& obj) {
        m1Matches = m1.matches(obj);
        m2Matches = m2.matches(obj);
        return m1Matches || m2Matches;
    }

    void describe(Description* description) {
        m1.describe(description);
        (*description) << " or ";
        m2.describe(description);
    }

    template<class T>
    void describeMismatch(Description* description, const T& obj) {
        m1.describeMismatch(description, obj);
        (*description) << " and ";
        m1.describeMismatch(description, obj);
    }

 private:
    M1 m1;
    M2 m2;

    bool m1Matches = false;
    bool m2Matches = false;
};

template<class M>
class NotMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M>::value,
                  "NotMatcher only supports other matchers as template args.");

 public:
    constexpr explicit NotMatcher(const M& matcher): matcher(matcher) {}

    template<class T>
    bool matches(const T& obj) {
        return !matcher.matches(obj);
    }

    void describe(Description* description) {
        (*description) << "not ";
        matcher.describe(description);
    }

    template<class T>
    void describeMismatch(Description* description, const T& obj) {
        matcher.describe(description);
    }
 private:
    M matcher;
};

}

}
