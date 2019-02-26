#ifndef KKTEST_EXTENSIONS_DEATH_INCLUDE_KKTEST_EXT_DEATH_IMPL_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_DEATH_INCLUDE_KKTEST_EXT_DEATH_IMPL_MATCHERS_HPP_

#include <kktest_ext/matchers_impl/comparison_matchers.hpp>
#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/death_impl/check_death.hpp>
#include <kktest_ext/death_impl/death_status.hpp>

namespace kktest {
namespace death {

namespace detail {
class ExitsMatcher;
class HasExitedMatcher;
template<class T> class HasExitedWithCodeMatcher;
template<class T> class HasOutputMatcher;
}

template<class T,
         class=typename std::enable_if<
                   std::is_base_of<matchers::Matcher, T>::value
               >::type>
detail::HasExitedWithCodeMatcher<T> hasExitedWithCode(const T& exitCodeMatcher){
    return detail::HasExitedWithCodeMatcher<T>(exitCodeMatcher);
}

template<class T,
         class=typename std::enable_if<
                   !std::is_base_of<matchers::Matcher, T>::value
               >::type>
detail::HasExitedWithCodeMatcher<matchers::detail::EqualityMatcher<T>>
        hasExitedWithCode(const T& exitCode) {
    return detail::HasExitedWithCodeMatcher
            <matchers::detail::EqualityMatcher<T>>(
                    matchers::isEqualTo(exitCode));
}

template<class T,
         class=typename std::enable_if<
                   std::is_base_of<matchers::Matcher, T>::value
               >::type>
detail::HasOutputMatcher<T> hasOutput(const T& outputMatcher) {
    return detail::HasOutputMatcher<T>(outputMatcher);
}

template<class T,
         class=typename std::enable_if<
                   !std::is_base_of<matchers::Matcher, T>::value
               >::type>
detail::HasOutputMatcher<matchers::detail::EqualityMatcher<T>>
        hasOutput(const T& output) {
    return detail::HasOutputMatcher<matchers::detail::EqualityMatcher<T>>(
        matchers::isEqualTo(output));
}

namespace detail {

void describeStatus(matchers::Description* description,
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

template<class CM, class OM>
class ExitsWithCodeAndOutputMatcher: public matchers::Matcher {
    static_assert(std::is_base_of<matchers::Matcher, CM>::value,
                  "ExitsWithCodeAndOutputMatcher only supports matchers as "
                  "template arguments.");
    static_assert(std::is_base_of<matchers::Matcher, OM>::value,
                  "ExitsWithCodeAndOutputMatcher only supports matchers as "
                  "template arguments.");
 public:
    ExitsWithCodeAndOutputMatcher(CM codeMatcher, OM outputMatcher):
            codeMatcher(std::move(codeMatcher)),
            outputMatcher(std::move(outputMatcher)) {}

    bool matches(Executable func) {
        status = checkDeath(func);
        codeMatcherMatched = codeMatcher.matches(status.getExitCode());
        return codeMatcherMatched && outputMatcher.matches(status.getOutput());
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end with code that is ";
        codeMatcher.describe(description);
        (*description) << " and where the final output is ";
        outputMatcher.describe(description);
    }

    void describeObject(matchers::Description* description, Executable func) {
        (*description) << status;
    }

    void describeMismatch(matchers::Description* description, Executable func) {
        if (!codeMatcherMatched) {
            describeStatus(description, status);
        } else {
            (*description) << "the program's end with valid return code,"
                              " but output is ";
            outputMatcher.describeMismatch(description, status.getOutput());
        }
    }

 private:
    CM codeMatcher;
    OM outputMatcher;
    bool codeMatcherMatched = false;
    DeathStatus status;
};

template<class M>
class ExitsWithCodeMatcher: public matchers::Matcher {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "ExitsWithCodeMatcher only supports matchers as template "
                  "arguments.");
 public:
    explicit ExitsWithCodeMatcher(M codeMatcher):
            codeMatcher(std::move(codeMatcher)) {}

    bool matches(Executable func) {
        status = checkDeath(func);
        return codeMatcher.matches(status.getExitCode());
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end with code that is ";
        codeMatcher.describe(description);
    }

    void describeObject(matchers::Description* description, Executable func) {
        (*description) << status;
    }

    void describeMismatch(matchers::Description* description, Executable func) {
        describeStatus(description, status);
    }

    template<class T,
             class=typename std::enable_if<
                       std::is_base_of<matchers::Matcher, T>::value
                   >::type>
    ExitsWithCodeAndOutputMatcher<M, T> withOutput(const T& outputMatcher) {
        return ExitsWithCodeAndOutputMatcher<M, T>(codeMatcher, outputMatcher);
    }

    template<class T,
             class=typename std::enable_if<
                       !std::is_base_of<matchers::Matcher, T>::value
                   >::type>
    ExitsWithCodeAndOutputMatcher<M, matchers::detail::EqualityMatcher<T>>
            withOutput(const T& output) {
        return ExitsWithCodeAndOutputMatcher
                <M, matchers::detail::EqualityMatcher<T>>(codeMatcher,
                        matchers::isEqualTo(output));
    }

 private:
    M codeMatcher;
    DeathStatus status;
};

template<class M>
class ExitsWithOutputMatcher: public matchers::Matcher {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "ExitsWithCodeMatcher only supports matchers as template "
                  "arguments.");
 public:
    explicit ExitsWithOutputMatcher(M outputMatcher):
            outputMatcher(std::move(outputMatcher)) {}

    bool matches(Executable func) {
        status = checkDeath(func);
        return outputMatcher.matches(status.getOutput());
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end, where the final output is ";
        outputMatcher.describe(description);
    }

    void describeObject(matchers::Description* description, Executable func) {
        (*description) << status;
    }

    void describeMismatch(matchers::Description* description, Executable func) {
        outputMatcher.describeMismatch(description, status.getOutput());
    }

    template<class T,
             class=typename std::enable_if<
                       std::is_base_of<matchers::Matcher, T>::value
                   >::type>
    ExitsWithCodeAndOutputMatcher<T, M> withCode(const T& codeMatcher) {
        return ExitsWithCodeAndOutputMatcher<T, M>(codeMatcher, outputMatcher);
    }

    template<class T,
             class=typename std::enable_if<
                       !std::is_base_of<matchers::Matcher, T>::value
                   >::type>
    ExitsWithCodeAndOutputMatcher<matchers::detail::EqualityMatcher<T>, M>
            withCode(const T& code) {
        return ExitsWithCodeAndOutputMatcher
                <matchers::detail::EqualityMatcher<T>, M>(
                        matchers::isEqualTo(code), outputMatcher);
    }

 private:
    M outputMatcher;
    DeathStatus status;
};

class ExitsMatcher: public matchers::Matcher {
 public:
    ExitsMatcher():
        zero(matchers::isZero), nonZero(matchers::isNot(matchers::isZero)) {}

    bool matches(Executable func) {
        status = checkDeath(func);
        return status.exited();
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end";
    }

    void describeObject(matchers::Description* description, Executable func) {
        (*description) << status;
    }

    void describeMismatch(matchers::Description* description, Executable func) {
        describeStatus(description, status);
    }

    template<class T,
             class=typename std::enable_if<
                       std::is_base_of<matchers::Matcher, T>::value
                   >::type>
    ExitsWithCodeMatcher<T> withCode(const T& codeMatcher) {
        return ExitsWithCodeMatcher<T>(codeMatcher);
    }

    template<class T,
             class=typename std::enable_if<
                       !std::is_base_of<matchers::Matcher, T>::value
                   >::type>
    ExitsWithCodeMatcher<matchers::detail::EqualityMatcher<T>>
            withCode(const T& code) {
        return ExitsWithCodeMatcher<matchers::detail::EqualityMatcher<T>>(
                matchers::isEqualTo(code));
    }

    template<class T,
             class=typename std::enable_if<
                       std::is_base_of<matchers::Matcher, T>::value
                   >::type>
    ExitsWithOutputMatcher<T> withOutput(const T& outputMatcher) {
        return ExitsWithOutputMatcher<T>(outputMatcher);
    }

    template<class T,
             class=typename std::enable_if<
                       !std::is_base_of<matchers::Matcher, T>::value
                   >::type>
    ExitsWithOutputMatcher<matchers::detail::EqualityMatcher<T>>
            withOutput(const T& output) {
        return ExitsWithOutputMatcher
                <matchers::detail::EqualityMatcher<T>>(
                        matchers::isEqualTo(output));
    }

    ExitsWithCodeMatcher<matchers::detail::IsZeroMatcher> zero;
    ExitsWithCodeMatcher
        <matchers::detail::NotMatcher<matchers::detail::IsZeroMatcher>> nonZero;

 private:
    DeathStatus status;
};

class HasExitedMatcher: public matchers::Matcher {
 public:
    bool matches(const DeathStatus& status) {
        return status.exited();
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end";
    }

    void describeMismatch(matchers::Description* description,
                          const DeathStatus& status) {
        describeStatus(description, status);
    }
};

template<class M>
class HasExitedWithCodeMatcher: public matchers::Matcher {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "HasExitedWithCodeMatcher only supports matchers as template "
                  "arguments.");
 public:
    explicit HasExitedWithCodeMatcher(M exitCodeMatcher):
            exitCodeMatcher(std::move(exitCodeMatcher)) {}

    bool matches(const DeathStatus& status) {
        return status.exited() && exitCodeMatcher.matches(status.getExitCode());
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end with code that is ";
        exitCodeMatcher.describe(description);
    }

    void describeMismatch(matchers::Description* description,
                          const DeathStatus& status) {
        describeStatus(description, status);
    }

 private:
    M exitCodeMatcher;
};

template<class M>
class HasOutputMatcher: public matchers::Matcher {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "HasOutputMatcher only supports matchers as template "
                  "arguments.");

 public:
    explicit HasOutputMatcher(M outputMatcher):
            outputMatcher(std::move(outputMatcher)) {}

    bool matches(const DeathStatus& status) {
        return outputMatcher.matches(status.getOutput());
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end, where the final output is ";
        outputMatcher.describe(description);
    }

    void describeMismatch(matchers::Description* description,
                          const DeathStatus& status) {
        outputMatcher.describeMismatch(description, status.getOutput());
    }

 private:
    M outputMatcher;
};

}

static detail::ExitsMatcher exits;

static detail::HasExitedMatcher hasExited;

static detail::HasExitedWithCodeMatcher<matchers::detail::IsZeroMatcher>
        hasExitedWithCodeZero(matchers::isZero);

static detail::HasExitedWithCodeMatcher
    <matchers::detail::NotMatcher<matchers::detail::IsZeroMatcher>>
        hasExitedWithNonZeroCode(matchers::isNot(matchers::isZero));

}
}

#endif
