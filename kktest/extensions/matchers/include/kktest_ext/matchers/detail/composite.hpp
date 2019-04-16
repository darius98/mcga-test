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
struct AndMatcher : StatefulMatcher<AndMatcherState<M1, M2>> {
    constexpr AndMatcher(M1 m1, M2 m2): m1(std::move(m1)), m2(std::move(m2)) {
    }

    template<class T>
    bool matches(const T& obj, AndMatcherState<M1, M2>* state) const {
        state->m1Matches = __matches(m1, &state->m1State, obj);
        state->m2Matches = __matches(m2, &state->m2State, obj);
        return state->m1Matches && state->m2Matches;
    }

    void describe(Description* description) const {
        m1.describe(description);
        (*description) << " and ";
        m2.describe(description);
    }

    void describeFailure(Description* description,
                         AndMatcherState<M1, M2>* state) const {
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
struct OrMatcher : StatefulMatcher<OrMatcherState<M1, M2>> {
    constexpr OrMatcher(M1 m1, M2 m2): m1(std::move(m1)), m2(std::move(m2)) {
    }

    template<class T>
    bool matches(const T& obj, OrMatcherState<M1, M2>* state) const {
        state->m1Matches = __matches(m1, &state->m1State, obj);
        state->m2Matches = __matches(m2, &state->m2State, obj);
        return state->m1Matches || state->m2Matches;
    }

    void describe(Description* description) const {
        m1.describe(description);
        (*description) << " or ";
        m2.describe(description);
    }

    void describeFailure(Description* description,
                         OrMatcherState<M1, M2>* state) const {
        __describeFailure(description, m1, &state->m1State);
        (*description) << " and ";
        __describeFailure(description, m2, &state->m2State);
    }

  private:
    M1 m1;
    M2 m2;
};

template<class M>
struct NotMatcher : StatelessMatcher {
    constexpr explicit NotMatcher(M matcher): matcher(std::move(matcher)) {
    }

    template<class T>
    bool matches(const T& obj) const {
        typename M::State state;
        return !__matches(matcher, &state, obj);
    }

    void describe(Description* description) const {
        (*description) << "not ";
        matcher.describe(description);
    }

    void describeFailure(Description* description) const {
        matcher.describe(description);
    }

  private:
    M matcher;
};

}