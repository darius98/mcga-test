#pragma once

#include "mcga/test_ext/matchers.hpp"
#include "check_death.hpp"
#include "death_status.hpp"

namespace mcga::test::death {

namespace detail {
class ExitsMatcher;
class HasExitedMatcher;
template<class T>
class HasExitedWithCodeMatcher;
}  // namespace detail

template<class T>
constexpr auto hasExitedWithCode(const T& exitCode) {
    if constexpr (std::is_base_of_v<matchers::Matcher, T>) {
        return detail::HasExitedWithCodeMatcher<T>(exitCode);
    } else {
        return detail::HasExitedWithCodeMatcher(matchers::isEqualTo(exitCode));
    }
}

namespace detail {

inline void describeStatus(matchers::Description* description,
                           const DeathStatus& status) {
    if (status.killedBySignal()) {
        (*description) << "the program's termination by signal "
                       << status.getSignal();
    } else if (status.exited()) {
        (*description) << "the program's end with code "
                       << status.getExitCode();
    } else {
        (*description) << "not a program exit";
    }
}

template<class M>
struct ExitsWithCodeState {
    DeathStatus status;
    typename M::State codeMatcherState;
};

template<class M>
class ExitsWithCodeMatcher
        : public matchers::StatefulMatcher<ExitsWithCodeState<M>> {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "ExitsWithCodeMatcher only supports matchers as template "
                  "arguments.");

  public:
    explicit constexpr ExitsWithCodeMatcher(M codeMatcher)
            : codeMatcher(std::move(codeMatcher)) {
    }

    bool matches(const Executable& func, ExitsWithCodeState<M>* state) const {
        state->status = checkDeath(func);
        return matchers::__matches(
          codeMatcher, &state->codeMatcherState, state->status.getExitCode());
    }

    void describe(matchers::Description* description) const {
        (*description) << "the program's end with code that is ";
        codeMatcher.describe(description);
    }

    void describeFailure(matchers::Description* description,
                         ExitsWithCodeState<M>* state) const {
        describeStatus(description, state->status);
    }

  private:
    M codeMatcher;
};

class ExitsMatcher : public matchers::StatefulMatcher<DeathStatus> {
  public:
    constexpr ExitsMatcher()
            : zero(matchers::isZero), nonZero(matchers::isNotZero) {
    }

    bool matches(const Executable& func, DeathStatus* state) const {
        *state = checkDeath(func);
        return state->exited();
    }

    void describe(matchers::Description* description) const {
        (*description) << "the program's end";
    }

    void describeFailure(matchers::Description* description,
                         DeathStatus* state) const {
        describeStatus(description, *state);
    }

    template<class T>
    auto withCode(const T& code) const {
        if constexpr (std::is_base_of_v<matchers::Matcher, T>) {
            return ExitsWithCodeMatcher<T>(code);
        } else {
            return ExitsWithCodeMatcher(matchers::isEqualTo(code));
        }
    }

    ExitsWithCodeMatcher<matchers::detail::IsZeroMatcher> zero;
    ExitsWithCodeMatcher<matchers::detail::IsNotZeroMatcher> nonZero;
};

class HasExitedMatcher : public matchers::StatefulMatcher<const DeathStatus*> {
  public:
    bool matches(const DeathStatus& status, const DeathStatus** state) const {
        *state = &status;
        return status.exited();
    }

    void describe(matchers::Description* description) const {
        (*description) << "the program's end";
    }

    void describeFailure(matchers::Description* description,
                         const DeathStatus** state) const {
        describeStatus(description, **state);
    }
};

template<class M>
class HasExitedWithCodeMatcher
        : public matchers::StatefulMatcher<const DeathStatus*> {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "HasExitedWithCodeMatcher only supports matchers as template "
                  "arguments.");

  public:
    explicit constexpr HasExitedWithCodeMatcher(M exitCodeMatcher)
            : exitCodeMatcher(std::move(exitCodeMatcher)) {
    }

    bool matches(const DeathStatus& status, const DeathStatus** state) const {
        *state = &status;
        typename M::State codeMatcherState;
        return status.exited()
          && matchers::__matches(
                 exitCodeMatcher, &codeMatcherState, status.getExitCode());
    }

    void describe(matchers::Description* description) const {
        (*description) << "the program's end with code that is ";
        exitCodeMatcher.describe(description);
    }

    void describeFailure(matchers::Description* description,
                         const DeathStatus** status) const {
        describeStatus(description, **status);
    }

  private:
    M exitCodeMatcher;
};

}  // namespace detail

constexpr detail::ExitsMatcher exits;

constexpr detail::HasExitedMatcher hasExited;

constexpr detail::HasExitedWithCodeMatcher
  hasExitedWithCodeZero(matchers::isZero);

constexpr detail::HasExitedWithCodeMatcher
  hasExitedWithNonZeroCode(matchers::isNotZero);

}  // namespace mcga::test::death
