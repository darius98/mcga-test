#include "extensions/death/include/kktest_ext/death_impl/matchers.hpp"

#include "core/src/export.hpp"

using namespace kktest::matchers;
using namespace std;

namespace kktest {
namespace death {

namespace detail {

KKTEST_EXPORT void describeStatus(Description* description,
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

KKTEST_EXPORT bool HasExitedMatcher::matches(const DeathStatus& status) {
    return status.exited();
}

KKTEST_EXPORT void HasExitedMatcher::describe(Description* description) {
    (*description) << "the program's end";
}

KKTEST_EXPORT void HasExitedMatcher::describeMismatch(
        Description* description, const DeathStatus& status) {
    describeStatus(description, status);
}

KKTEST_EXPORT ExitsMatcher::ExitsMatcher():
        zero(isEqualTo(0)), nonZero(isNotEqualTo(0)) {}

KKTEST_EXPORT bool ExitsMatcher::matches(const function<void()>& func) {
    status = checkDeath(func);
    return status.exited();
}

KKTEST_EXPORT void ExitsMatcher::describe(Description* description) {
    (*description) << "the program's end";
}

KKTEST_EXPORT void ExitsMatcher::describeObject(Description* description,
                                                const function<void()>& func) {
    (*description) << status;
}

KKTEST_EXPORT void ExitsMatcher::describeMismatch(
        Description* description, const function<void()>& func) {
    describeStatus(description, status);
}

}

KKTEST_EXPORT detail::ExitsMatcher exits;

KKTEST_EXPORT detail::HasExitedMatcher hasExited;

KKTEST_EXPORT detail::HasExitedWithCodeMatcher
        <matchers::detail::EqualityMatcher<int>>
            hasExitedWithCodeZero(isEqualTo(0));

KKTEST_EXPORT detail::HasExitedWithCodeMatcher
        <matchers::detail::NonEqualityMatcher<int>>
            hasExitedWithNonZeroCode(isNotEqualTo(0));

}
}
