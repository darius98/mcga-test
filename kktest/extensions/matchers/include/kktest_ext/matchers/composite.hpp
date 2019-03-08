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
struct AndMatcherState {
    bool m1Matches = false;
    bool m2Matches = false;
    typename M1::State m1State;
    typename M2::State m2State;
};

template<class M1, class M2>
class AndMatcher: public StatefulMatcher<AndMatcherState<M1, M2>> {
    static_assert(std::is_base_of_v<Matcher, M1>,
                  "AndMatcher only supports other matchers as template args.");
    static_assert(std::is_base_of_v<Matcher, M2>,
                  "AndMatcher only supports other matchers as template args.");
 public:
    constexpr AndMatcher(const M1& m1, const M2& m2): m1(m1), m2(m2) {}

    template<class T>
    bool matches(const T& obj, AndMatcherState<M1, M2>* state) const {
        if constexpr (M1::HasState) {
            state->m1Matches = m1.matches(obj, &state->m1State);
        } else {
            state->m1Matches = m1.matches(obj);
        }
        if constexpr (M2::HasState) {
            state->m2Matches = m2.matches(obj, &state->m2State);
        } else {
            state->m2Matches = m2.matches(obj);
        }
        return state->m1Matches && state->m2Matches;
    }

    void describe(Description* description) const override {
        m1.describe(description);
        (*description) << " and ";
        m2.describe(description);
    }

    void describeFailure(Description* description,
                         AndMatcherState<M1, M2>* state) const override {
        if (state->m1Matches) {
            m2.describeFailure(description, &state->m2Matches);
        } else {
            m1.describeFailure(description, &state->m1Matches);
        }
    }

 private:
    M1 m1;
    M2 m2;
};

template<class M1, class M2>
struct OrMatcherState {
    bool m1Matches = false;
    bool m2Matches = false;
    typename M1::State m1State;
    typename M2::State m2State;
};

template<class M1, class M2>
class OrMatcher: public StatefulMatcher<OrMatcherState<M1, M2>> {
    static_assert(std::is_base_of<Matcher, M1>::value,
                  "OrMatcher only supports other matchers as template args.");
    static_assert(std::is_base_of<Matcher, M2>::value,
                  "OrMatcher only supports other matchers as template args.");
 public:
    constexpr OrMatcher(const M1& m1, const M2& m2): m1(m1), m2(m2) {}

    template<class T>
    bool matches(const T& obj, OrMatcherState<M1, M2>* state) const {
        if constexpr (M1::HasState) {
            state->m1Matches = m1.matches(obj, &state->m1State);
        } else {
            state->m1Matches = m1.matches(obj);
        }
        if constexpr (M2::HasState) {
            state->m2Matches = m2.matches(obj, &state->m2State);
        } else {
            state->m2Matches = m2.matches(obj);
        }
        return state->m1Matches || state->m2Matches;
    }

    void describe(Description* description) const override {
        m1.describe(description);
        (*description) << " or ";
        m2.describe(description);
    }

    void describeFailure(Description* description,
                         OrMatcherState<M1, M2>* state) const override {
        m1.describeFailure(description, &state->m1State);
        (*description) << " and ";
        m2.describeFailure(description, &state->m2State);
    }

 private:
    M1 m1;
    M2 m2;
};

template<class M>
class NotMatcher: public StatelessMatcher {
    static_assert(std::is_base_of<Matcher, M>::value,
                  "NotMatcher only supports other matchers as template args.");

 public:
    constexpr explicit NotMatcher(const M& matcher): matcher(matcher) {}

    template<class T>
    bool matches(const T& obj) const {
        if constexpr (M::HasState) {
            typename M::State state;
            return !matcher.matches(obj, &state);
        } else {
            return !matcher.matches(obj);
        }
    }

    void describe(Description* description) const override {
        (*description) << "not ";
        matcher.describe(description);
    }

    void describeFailure(Description* description) const override {
        matcher.describe(description);
    }
 private:
    M matcher;
};

}

}
