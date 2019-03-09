#pragma once

#include <kktest_ext/matchers/comparison.hpp>

namespace kktest::matchers::detail {

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
        state->m1Matches = __matches(m1, &state->m1State, obj);
        state->m2Matches = __matches(m2, &state->m2State, obj);
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
            __describeFailure(description, m2, &state->m2State);
        } else {
            __describeFailure(description, m1, &state->m1State);
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
        state->m1Matches = __matches(m1, &state->m1State, obj);
        state->m2Matches = __matches(m2, &state->m2State, obj);
        return state->m1Matches || state->m2Matches;
    }

    void describe(Description* description) const override {
        m1.describe(description);
        (*description) << " or ";
        m2.describe(description);
    }

    void describeFailure(Description* description,
                         OrMatcherState<M1, M2>* state) const override {
        __describeFailure(description, m1, &state->m1State);
        (*description) << " and ";
        __describeFailure(description, m2, &state->m2State);
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
        typename M::State state;
        return !__matches(matcher, &state, obj);
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