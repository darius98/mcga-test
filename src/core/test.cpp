#include "test.hpp"

#include <algorithm>

using std::move;
using std::size_t;
using std::string;
using std::vector;

namespace mcga::test {

void Test::ExecutionInfo::fail(const string& _failure, double _timeTicks) {
    if (passed) {
        passed = false;
        failure = _failure;
        timeTicks = _timeTicks;
    }
}

Test::Test(TestConfig config, Executable body, GroupPtr group, int id)
        : TestConfig(move(config)), body(move(body)), group(move(group)),
          id(id) {
}

const string& Test::getDescription() const {
    return description;
}

bool Test::isOptional() const {
    return optional || group->isOptional();
}

double Test::getTimeTicksLimit() const {
    return timeTicksLimit;
}

std::size_t Test::getNumAttempts() const {
    return attempts;
}

std::size_t Test::getNumRequiredPassedAttempts() const {
    return requiredPassedAttempts;
}

GroupPtr Test::getGroup() const {
    return group;
}

vector<GroupPtr> Test::getGroupStack() const {
    vector<GroupPtr> groupStack(1, getGroup());
    while (groupStack.back()->getParentGroup() != nullptr) {
        groupStack.push_back(groupStack.back()->getParentGroup());
    }
    reverse(groupStack.begin(), groupStack.end());
    return groupStack;
}

int Test::getId() const {
    return id;
}

void Test::run() const {
    body();
}

bool Test::isExecuted() const {
    return executions.size() == attempts;
}

bool Test::isPassed() const {
    return getNumPassedAttempts() >= getNumRequiredPassedAttempts();
}

size_t Test::getNumPassedAttempts() const {
    int numPassedExecutions = 0;
    for (const auto& info: executions) {
        numPassedExecutions += (info.passed ? 1 : 0);
    }
    return numPassedExecutions;
}

double Test::getAvgTimeTicksForExecution() const {
    int numCountedExecutions = 0;
    double total = 0;
    for (const auto& info: executions) {
        if (info.timeTicks != -1) {
            total += info.timeTicks;
            numCountedExecutions += 1;
        }
    }
    if (numCountedExecutions > 0) {
        return total / numCountedExecutions;
    }
    return -1;
}

double Test::getTotalTimeTicks() const {
    int numCountedExecutions = 0;
    double totalTimeTicks = 0;
    for (const auto& info: executions) {
        if (info.timeTicks != -1) {
            totalTimeTicks += info.timeTicks;
            numCountedExecutions += 1;
        }
    }
    if (numCountedExecutions == 0) {
        totalTimeTicks = -1;
    }
    return totalTimeTicks;
}

string Test::getLastFailure() const {
    for (auto it = executions.rbegin(); it != executions.rend(); ++it) {
        if (!it->passed) {
            return it->failure;
        }
    }
    return "";
}

const vector<Test::ExecutionInfo>& Test::getExecutions() const {
    return executions;
}

void Test::addExecution(const ExecutionInfo& info) {
    executions.push_back(info);
}

}  // namespace mcga::test
