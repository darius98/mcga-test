#include "kktest/core/src/executed_test.hpp"

using namespace std;

namespace kktest {

ExecutedTest::Info::Info(string&& failure):
        timeTicks(-1.0), passed(false), failure(move(failure)) {}

void ExecutedTest::Info::fail(const string& failure) {
    if (passed) {
        passed = false;
        this->failure = failure;
    }
}

ExecutedTest::ExecutedTest(Test&& test, vector<Info>&& executions):
        Test(move(test)), executions(move(executions)) {}

bool ExecutedTest::isPassed() const {
    return getNumPassedAttempts() >= getNumRequiredPassedAttempts();
}

int ExecutedTest::getNumPassedAttempts() const {
    if (numPassedExecutions == -1) {
        numPassedExecutions = 0;
        for (const auto& info: executions) {
            numPassedExecutions += info.passed;
        }
    }
    return numPassedExecutions;
}

double ExecutedTest::getAvgTimeTicksForExecution() const {
    if (avgTimeTicks == -2) {
        int numCountedExecutions = 0;
        double totalTimeTicks = 0;
        for (const auto& info: executions) {
            if (info.timeTicks != -1) {
                totalTimeTicks += info.timeTicks;
                numCountedExecutions += 1;
            }
        }
        if (numCountedExecutions > 0) {
            avgTimeTicks = totalTimeTicks / numCountedExecutions;
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
    for (int i = static_cast<int>(executions.size()); i >= 0; -- i) {
        if (!executions[i].passed) {
            return executions[i].failure;
        }
    }
    return "";
}

const vector<ExecutedTest::Info>& ExecutedTest::getExecutions() const {
    return executions;
}

}
