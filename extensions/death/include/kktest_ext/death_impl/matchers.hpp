#ifndef KKTEST_EXTENSIONS_DEATH_KKTEST_EXT_DEATH_IMPL_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_DEATH_KKTEST_EXT_DEATH_IMPL_MATCHERS_HPP_

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

extern KKTEST_API detail::ExitsMatcher exits;

extern KKTEST_API detail::HasExitedMatcher hasExited;

extern KKTEST_API detail::HasExitedWithCodeMatcher<
        matchers::detail::EqualityMatcher<int>> hasExitedWithCodeZero;

extern KKTEST_API detail::HasExitedWithCodeMatcher<
        matchers::detail::NonEqualityMatcher<int>> hasExitedWithNonZeroCode;

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

void KKTEST_API describeStatus(matchers::Description* description,
                    const DeathStatus& status);

template<class CM, class OM>
class ExitsWithCodeAndOutputMatcher: public matchers::Matcher {
    static_assert(std::is_base_of<matchers::Matcher, CM>::value,
                  "ExitsWithCodeAndOutputMatcher only supports matchers as "
                  "template arguments.");
    static_assert(std::is_base_of<matchers::Matcher, OM>::value,
                  "ExitsWithCodeAndOutputMatcher only supports matchers as "
                  "template arguments.");
 public:
    ExitsWithCodeAndOutputMatcher(CM _codeMatcher, OM _outputMatcher):
            codeMatcher(std::move(_codeMatcher)),
            outputMatcher(std::move(_outputMatcher)) {}

    bool matches(const std::function<void()>& func) {
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

    void describeObject(matchers::Description* description,
                        const std::function<void()>& func) {
        (*description) << status;
    }

    void describeMismatch(matchers::Description* description,
                          const std::function<void()>& func) {
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
    explicit ExitsWithCodeMatcher(M _codeMatcher):
            codeMatcher(std::move(_codeMatcher)) {}

    bool matches(const std::function<void()>& func) {
        status = checkDeath(func);
        return codeMatcher.matches(status.getExitCode());
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end with code that is ";
        codeMatcher.describe(description);
    }

    void describeObject(matchers::Description* description,
                        const std::function<void()>& func) {
        (*description) << status;
    }

    void describeMismatch(matchers::Description* description,
                          const std::function<void()>& func) {
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
    explicit ExitsWithOutputMatcher(M _outputMatcher):
            outputMatcher(std::move(_outputMatcher)) {}

    bool matches(const std::function<void()>& func) {
        status = checkDeath(func);
        return outputMatcher.matches(status.getOutput());
    }

    void describe(matchers::Description* description) {
        (*description) << "the program's end, where the final output is ";
        outputMatcher.describe(description);
    }

    void describeObject(matchers::Description* description,
                        const std::function<void()>& func) {
        (*description) << status;
    }

    void describeMismatch(matchers::Description* description,
                          const std::function<void()>& func) {
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
    ExitsWithCodeAndOutputMatcher
            <matchers::detail::EqualityMatcher<T>, M> withCode(
                    const T& code) {
        return ExitsWithCodeAndOutputMatcher
                <matchers::detail::EqualityMatcher<T>, M>(
                        matchers::isEqualTo(code), outputMatcher);
    }

 private:
    M outputMatcher;
    DeathStatus status;
};

class KKTEST_API ExitsMatcher: public matchers::Matcher {
 public:
    ExitsMatcher();

    bool matches(const std::function<void()>& func);

    void describe(matchers::Description* description);

    void describeObject(matchers::Description* description,
                        const std::function<void()>& func);

    void describeMismatch(matchers::Description* description,
                          const std::function<void()>& func);

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
    ExitsWithCodeMatcher<
            matchers::detail::EqualityMatcher<T>> withCode(const T& code) {
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
    ExitsWithOutputMatcher<
            matchers::detail::EqualityMatcher<T>
            > withOutput(const T& output) {
        return ExitsWithOutputMatcher
                <matchers::detail::EqualityMatcher<T>>(
                        matchers::isEqualTo(output));
    }

    ExitsWithCodeMatcher<matchers::detail::EqualityMatcher<int>> zero;
    ExitsWithCodeMatcher<matchers::detail::NonEqualityMatcher<int>>
            nonZero;

 private:
    DeathStatus status;
};

class KKTEST_API HasExitedMatcher: public matchers::Matcher {
 public:
    bool matches(const DeathStatus& status);

    void describe(matchers::Description* description);

    void describeMismatch(matchers::Description* description,
                          const DeathStatus& status);
};

template<class M>
class HasExitedWithCodeMatcher: public matchers::Matcher {
    static_assert(std::is_base_of<matchers::Matcher, M>::value,
                  "HasExitedWithCodeMatcher only supports matchers as template "
                  "arguments.");
 public:
    explicit HasExitedWithCodeMatcher(M _exitCodeMatcher):
            exitCodeMatcher(std::move(_exitCodeMatcher)) {}

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
    explicit HasOutputMatcher(M _outputMatcher):
            outputMatcher(std::move(_outputMatcher)) {}

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
}
}

#endif
