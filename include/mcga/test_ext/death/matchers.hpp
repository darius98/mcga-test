#pragma once

#include "check_death.hpp"
#include "death_status.hpp"
#include "mcga/matchers.hpp"

namespace mcga::test::death {

namespace internal {
class ExitsMatcher;
class HasExitedMatcher;
template<class T>
class HasExitedWithCodeMatcher;
}  // namespace internal

template<class T>
constexpr auto hasExitedWithCode(const T& exitCode) {
    if constexpr (matchers::isMatcher<T>) {
        return internal::HasExitedWithCodeMatcher<T>(exitCode);
    } else {
        return internal::HasExitedWithCodeMatcher(
          matchers::isEqualTo(exitCode));
    }
}

namespace internal {

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
class ExitsWithCodeMatcher : public matchers::Matcher {
    static_assert(matchers::isMatcher<M>,
                  "ExitsWithCodeMatcher only supports matchers as template "
                  "arguments.");

  public:
    explicit constexpr ExitsWithCodeMatcher(M codeMatcher)
            : codeMatcher(std::move(codeMatcher)) {
    }

    bool matches(const Executable& func) {
        status = checkDeath(func);
        return codeMatcher.matches(status.getExitCode());
    }

    void describe(matchers::Description* description) const {
        (*description) << "the program's end with code that is ";
        codeMatcher.describe(description);
    }

    void describeFailure(matchers::Description* description) const {
        describeStatus(description, status);
    }

  private:
    DeathStatus status;
    M codeMatcher;
};

class ExitsMatcher : public matchers::Matcher {
  public:
    constexpr ExitsMatcher()
            : zero(matchers::isZero), nonZero(matchers::isNotZero) {
    }

    bool matches(const Executable& func) {
        status = checkDeath(func);
        return status.exited();
    }

    void describe(matchers::Description* description) const {
        (*description) << "the program's end";
    }

    void describeFailure(matchers::Description* description) const {
        describeStatus(description, status);
    }

    template<class T>
    auto withCode(const T& code) const {
        if constexpr (matchers::isMatcher<T>) {
            return ExitsWithCodeMatcher<T>(code);
        } else {
            return ExitsWithCodeMatcher(matchers::isEqualTo(code));
        }
    }

    ExitsWithCodeMatcher<matchers::internal::IsZeroMatcher> zero;
    ExitsWithCodeMatcher<matchers::internal::IsNotZeroMatcher> nonZero;

  private:
    DeathStatus status;
};

class HasExitedMatcher : public matchers::Matcher {
  public:
    bool matches(const DeathStatus& stat) {
        status = &stat;
        return status->exited();
    }

    void describe(matchers::Description* description) const {
        (*description) << "the program's end";
    }

    void describeFailure(matchers::Description* description) const {
        describeStatus(description, *status);
    }

  private:
    const DeathStatus* status = nullptr;
};

template<class M>
class HasExitedWithCodeMatcher : public matchers::Matcher {
    static_assert(matchers::isMatcher<M>,
                  "HasExitedWithCodeMatcher only supports matchers as template "
                  "arguments.");

  public:
    explicit constexpr HasExitedWithCodeMatcher(M exitCodeMatcher)
            : exitCodeMatcher(std::move(exitCodeMatcher)) {
    }

    bool matches(const DeathStatus& stat) {
        status = &stat;
        return status->exited()
          && exitCodeMatcher.matches(status->getExitCode());
    }

    void describe(matchers::Description* description) const {
        (*description) << "the program's end with code that is ";
        exitCodeMatcher.describe(description);
    }

    void describeFailure(matchers::Description* description) const {
        describeStatus(description, *status);
    }

  private:
    const DeathStatus* status = nullptr;
    M exitCodeMatcher;
};

}  // namespace internal

constexpr internal::ExitsMatcher exits;

constexpr internal::HasExitedMatcher hasExited;

constexpr internal::HasExitedWithCodeMatcher
  hasExitedWithCodeZero(matchers::isZero);

constexpr internal::HasExitedWithCodeMatcher
  hasExitedWithNonZeroCode(matchers::isNotZero);

}  // namespace mcga::test::death
