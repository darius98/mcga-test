#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_COMPOSITE_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_COMPOSITE_MATCHERS_HPP_

#include <kktest_ext/matchers_impl/comparison_matchers.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>

namespace kktest {
namespace matchers {

template<
    class A,
    class B,
    class=typename std::enable_if<std::is_base_of<Matcher, A>::value>::type,
    class=typename std::enable_if<std::is_base_of<Matcher, B>::value>::type>
detail::AndMatcher<A, B> both(const A& a, const B& b) {
    return detail::AndMatcher<A, B>(a, b);
}

template<
    class A,
    class B,
    class=typename std::enable_if<std::is_base_of<Matcher, A>::value>::type,
    class=typename std::enable_if<!std::is_base_of<Matcher, B>::value>::type>
detail::AndMatcher<A, detail::EqualityMatcher<B>> both(const A& a, const B& b) {
    return detail::AndMatcher<A, detail::EqualityMatcher<B>>(a, isEqualTo(b));
}

template<
    class A,
    class B,
    class=typename std::enable_if<!std::is_base_of<Matcher, A>::value>::type,
    class=typename std::enable_if<std::is_base_of<Matcher, B>::value>::type>
detail::AndMatcher<detail::EqualityMatcher<A>, B> both(const A& a, const B& b) {
    return detail::AndMatcher<detail::EqualityMatcher<A>, B>(isEqualTo(a), b);
}

template<
    class A,
    class B,
    class=typename std::enable_if<!std::is_base_of<Matcher, A>::value>::type,
    class=typename std::enable_if<!std::is_base_of<Matcher, B>::value>::type>
detail::AndMatcher<detail::EqualityMatcher<A>, detail::EqualityMatcher<B>>
        both(const A& a, const B& b) {
    return detail::AndMatcher<detail::EqualityMatcher<A>,
                              detail::EqualityMatcher<B>
                             >(isEqualTo(a), isEqualTo(b));
}

template<
    class A,
    class B,
    class=typename std::enable_if<std::is_base_of<Matcher, A>::value>::type,
    class=typename std::enable_if<std::is_base_of<Matcher, B>::value>::type>
detail::OrMatcher<A, B> either(const A& a, const B& b) {
    return detail::OrMatcher<A, B>(a, b);
}

template<
    class A,
    class B,
    class=typename std::enable_if<std::is_base_of<Matcher, A>::value>::type,
    class=typename std::enable_if<!std::is_base_of<Matcher, B>::value>::type>
detail::OrMatcher<A, detail::EqualityMatcher<B>> either(const A& a,
                                                        const B& b) {
    return detail::OrMatcher<A, detail::EqualityMatcher<B>>(a, isEqualTo(b));
}

template<
    class A,
    class B,
    class=typename std::enable_if<!std::is_base_of<Matcher, A>::value>::type,
    class=typename std::enable_if<std::is_base_of<Matcher, B>::value>::type>
detail::OrMatcher<detail::EqualityMatcher<A>, B> either(const A& a,
                                                        const B& b) {
    return detail::OrMatcher<detail::EqualityMatcher<A>, B>(isEqualTo(a), b);
}

template<
    class A,
    class B,
    class=typename std::enable_if<!std::is_base_of<Matcher, A>::value>::type,
    class=typename std::enable_if<!std::is_base_of<Matcher, B>::value>::type>
detail::OrMatcher<detail::EqualityMatcher<A>,
                  detail::EqualityMatcher<B>> either(const A& a, const B& b) {
    return detail::OrMatcher<detail::EqualityMatcher<A>,
                             detail::EqualityMatcher<B>>(isEqualTo(a),
                                                         isEqualTo(b));
}

template<
    class T,
    class=typename std::enable_if<std::is_base_of<Matcher, T>::value>::type>
detail::NotMatcher<T> isNot(const T& x) {
    return detail::NotMatcher<T>(x);
}

template<
    class T,
    class=typename std::enable_if<!std::is_base_of<Matcher, T>::value>::type>
detail::NonEqualityMatcher<T> isNot(const T& x) {
    return isNotEqualTo(x);
}

namespace detail {

template<class M1, class M2>
class AndMatcher: public Matcher {
    static_assert(std::is_base_of<Matcher, M1>::value,
                  "AndMatcher only supports other matchers as template args.");
    static_assert(std::is_base_of<Matcher, M2>::value,
                  "AndMatcher only supports other matchers as template args.");

 public:
    AndMatcher(const M1& _m1, const M2& _m2): m1(_m1), m2(_m2) {}

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
    OrMatcher(const M1& _m1, const M2& _m2): m1(_m1), m2(_m2) {}

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
    explicit NotMatcher(const M& _matcher): matcher(_matcher) {}

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
}

#endif
