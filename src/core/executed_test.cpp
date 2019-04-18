#include "executed_test.hpp"

using std::move;
using std::string;
using std::vector;

namespace mcga::test {

ExecutedTest::Info::Info(string&& failure)
        : passed(false), failure(move(failure)) {
}

void ExecutedTest::Info::fail(const string& _failure) {
    if (passed) {
        passed = false;
        failure = _failure;
    }
}

ExecutedTest::ExecutedTest(Test test, vector<Info>&& executions)
        : Test(move(test)), executions(move(executions)) {
}

bool ExecutedTest::isPassed() const {
    return getNumPassedAttempts() >= getNumRequiredPassedAttempts();
}

size_t ExecutedTest::getNumPassedAttempts() const {
    if (numPassedExecutions == static_cast<size_t>(-1)) {
        numPassedExecutions = 0;
        for (const auto& info: executions) {
            numPassedExecutions += (info.passed ? 1 : 0);
        }
    }
    return numPassedExecutions;
}

double ExecutedTest::getAvgTimeTicksForExecution() const {
    if (avgTimeTicks == -2) {
        int numCountedExecutions = 0;
        double total = 0;
        for (const auto& info: executions) {
            if (info.timeTicks != -1) {
                total += info.timeTicks;
                numCountedExecutions += 1;
            }
        }
        if (numCountedExecutions > 0) {
            avgTimeTicks = total / numCountedExecutions;
        } else {
            avgTimeTicks = -1;
        }
    }
    return avgTimeTicks;
}

double ExecutedTest::getTotalTimeTicks() const {
    if (totalTimeTicks == -2) {
        int numCountedExecutions = 0;
        totalTimeTicks = 0;
        for (const auto& info: executions) {
            if (info.timeTicks != -1) {
                totalTimeTicks += info.timeTicks;
                numCountedExecutions += 1;
            }
        }
        if (numCountedExecutions == 0) {
            totalTimeTicks = -1;
        }
    }
    return totalTimeTicks;
}

string ExecutedTest::getLastFailure() const {
    for (auto it = executions.rbegin(); it != executions.rend(); ++it) {
        if (!it->passed) {
            return it->failure;
        }
    }
    return "";
}

const vector<ExecutedTest::Info>& ExecutedTest::getExecutions() const {
    return executions;
}

}  // namespace mcga::test
