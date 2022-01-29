#include "test.hpp"

#include "extension_api.hpp"

namespace mcga::test {

void Test::ExecutionInfo::fail(const String& failureMessage,
                               std::optional<Context> failureContext) {
    if (isPassed()) {
        status = FAILED;
        message = failureMessage;
        context = failureContext;
    }
}

void Test::ExecutionInfo::merge(ExecutionInfo&& other) {
    if (isPassed() || (status != SKIPPED && other.status == SKIPPED)) {
        status = other.status;
        message = std::move(other.message);
        context = other.context;
        return;
    }
}

bool Test::ExecutionInfo::isPassed() const {
    return status == PASSED;
}

Test::Test(TestConfig config, Executable body, GroupPtr group, int id)
        : TestConfig(std::move(config)), body(std::move(body)),
          group(std::move(group)), id(id) {
    if (requiredPassedAttempts == -1) {
        requiredPassedAttempts = attempts;
    }
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

int Test::getId() const {
    return id;
}

const Executable& Test::getBody() const {
    return body;
}

bool Test::isFinished() const {
    return getNumExecutedAttempts() == attempts;
}

bool Test::isPassed() const {
    return numPassedAttempts >= requiredPassedAttempts;
}

bool Test::isSkipped() const {
    return numPassedAttempts < requiredPassedAttempts && numSkippedAttempts > 0
      && numPassedAttempts + numSkippedAttempts == attempts;
}

bool Test::isFailed() const {
    return !isPassed() && !isSkipped();
}

size_t Test::getNumExecutedAttempts() const {
    return numExecutedAttempts;
}

size_t Test::getNumPassedAttempts() const {
    return numPassedAttempts;
}

size_t Test::getNumSkippedAttempts() const {
    return numSkippedAttempts;
}

double Test::getAvgTimeTicksForExecution() const {
    if (numTimedAttempts == 0) {
        return -1;
    }
    return trackedExecutionTimeTicks / numTimedAttempts;
}

double Test::getTotalTimeTicks() const {
    if (numTimedAttempts == 0) {
        return -1;
    }
    return trackedExecutionTimeTicks;
}

const Test::ExecutionInfo& Test::getLastExecution() const {
    return lastExecution;
}

const std::optional<Test::ExecutionInfo>& Test::getLastFailedExecution() const {
    return lastFailedExecution;
}

const std::optional<Test::ExecutionInfo>&
  Test::getLastSkippedExecution() const {
    return lastSkippedExecution;
}

void Test::addExecution(ExecutionInfo info, ExtensionApi* api) {
    if (info.status == ExecutionInfo::PASSED) {
        numPassedAttempts += 1;
    }
    if (info.status == ExecutionInfo::FAILED) {
        lastFailedExecution = info;
    }
    if (info.status == ExecutionInfo::SKIPPED) {
        numSkippedAttempts += 1;
        lastSkippedExecution = info;
    }
    numExecutedAttempts += 1;
    if (info.timeTicks != -1) {
        numTimedAttempts += 1;
        trackedExecutionTimeTicks += info.timeTicks;
    }
    lastExecution = std::move(info);
    api->afterTestExecution(*this);
}

}  // namespace mcga::test
