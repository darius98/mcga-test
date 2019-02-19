#include "extensions/death/include/kktest_ext/death_impl/matchers.hpp"

using namespace kktest::matchers;
using namespace std;

namespace kktest {
namespace death {

namespace detail {

void describeStatus(Description* description, const DeathStatus& status) {
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

bool HasExitedMatcher::matches(const DeathStatus& status) {
    return status.exited();
}

void HasExitedMatcher::describe(Description* description) {
    (*description) << "the program's end";
}

void HasExitedMatcher::describeMismatch(Description* description,
                                        const DeathStatus& status) {
    describeStatus(description, status);
}

ExitsMatcher::ExitsMatcher():
        zero(isEqualTo(0)), nonZero(isNotEqualTo(0)) {}

bool ExitsMatcher::matches(const function<void()>& func) {
    status = checkDeath(func);
    return status.exited();
}

void ExitsMatcher::describe(Description* description) {
    (*description) << "the program's end";
}

void ExitsMatcher::describeObject(Description* description,
                                  const function<void()>& func) {
    (*description) << status;
}

void ExitsMatcher::describeMismatch(Description* description,
                                    const function<void()>& func) {
    describeStatus(description, status);
}

}

detail::ExitsMatcher exits;

detail::HasExitedMatcher hasExited;

detail::HasExitedWithCodeMatcher<matchers::detail::EqualityMatcher<int>>
            hasExitedWithCodeZero(isEqualTo(0));

detail::HasExitedWithCodeMatcher<matchers::detail::NonEqualityMatcher<int>>
            hasExitedWithNonZeroCode(isNotEqualTo(0));

}
}
