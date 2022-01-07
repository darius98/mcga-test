#include "test.hpp"

#include <algorithm>

namespace mcga::test {

void Test::ExecutionInfo::fail(const std::string& _failure,
                               std::optional<Context> _failureContext,
                               double _timeTicks) {
    if (passed) {
        passed = false;
        failure = _failure;
        failureContext = std::move(_failureContext);
        timeTicks = _timeTicks;
    }
}

Test::Test(TestConfig config, Executable body, GroupPtr group, int id)
        : TestConfig(std::move(config)), body(std::move(body)),
          group(std::move(group)), id(id) {
}

const std::string& Test::getDescription() const {
    return description;
}

const Context& Test::getContext() const {
    return body.context;
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

std::vector<GroupPtr> Test::getGroupStack() const {
    std::vector<GroupPtr> groupStack(1, getGroup());
    while (groupStack.back()->getParentGroup() != nullptr) {
        groupStack.push_back(groupStack.back()->getParentGroup());
    }
    reverse(groupStack.begin(), groupStack.end());
    return groupStack;
}

int Test::getId() const {
    return id;
}

const Executable& Test::getBody() const {
    return body;
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

std::optional<Test::ExecutionInfo> Test::getLastFailure() const {
    for (auto it = executions.rbegin(); it != executions.rend(); ++it) {
        if (!it->passed) {
            return *it;
        }
    }
    return std::nullopt;
}

const std::vector<Test::ExecutionInfo>& Test::getExecutions() const {
    return executions;
}

void Test::addExecution(const ExecutionInfo& info) {
    executions.push_back(info);
}

}  // namespace mcga::test
