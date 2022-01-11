#include "test.hpp"

#include <algorithm>

namespace mcga::test {

void Test::ExecutionInfo::fail(const String& failureMessage,
                               std::optional<Context> failureContext) {
    if (isPassed()) {
        status = FAILED;
        message = failureMessage;
        context = std::move(failureContext);
    }
}

void Test::ExecutionInfo::merge(ExecutionInfo&& other) {
    if (isPassed() || (status != SKIPPED && other.status == SKIPPED)) {
        status = other.status;
        message = std::move(other.message);
        context = std::move(other.context);
        return;
    }
}

bool Test::ExecutionInfo::isPassed() const {
    return status == PASSED;
}

Test::Test(TestConfig config, Executable body, GroupPtr group, int id)
        : TestConfig(std::move(config)), body(std::move(body)),
          group(std::move(group)), id(id) {
}

const String& Test::getDescription() const {
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

bool Test::isFinished() const {
    return executions.size() == attempts;
}

bool Test::isPassed() const {
    return numPassedExecutions >= requiredPassedAttempts;
}

bool Test::isSkipped() const {
    return numPassedExecutions < requiredPassedAttempts
      && numSkippedExecutions > 0
      && numPassedExecutions + numSkippedExecutions == attempts;
}

bool Test::isFailed() const {
    return !isPassed() && !isSkipped();
}

size_t Test::getNumPassedAttempts() const {
    return numPassedExecutions;
}

size_t Test::getNumSkippedAttempts() const {
    return numSkippedExecutions;
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

std::optional<Test::ExecutionInfo>
  Test::getLastExecutionWithStatus(ExecutionInfo::Status status) const {
    for (auto it = executions.rbegin(); it != executions.rend(); ++it) {
        if (it->status == status) {
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
    if (info.status == ExecutionInfo::PASSED) {
        numPassedExecutions += 1;
    }
    if (info.status == ExecutionInfo::SKIPPED) {
        numSkippedExecutions += 1;
    }
}

}  // namespace mcga::test
