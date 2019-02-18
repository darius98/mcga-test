#include "core/src/test.hpp"

#include "core/src/errors.hpp"
#include "core/src/group.hpp"

using namespace kktest::interproc;
using namespace std;

namespace kktest {

ExecutionInfo::ExecutionInfo(const string& errorMessage):
        timeTicks(-1.0), passed(false), failure(errorMessage) {}

ExecutionInfo::ExecutionInfo(Message& message) {
    MessageStatus status;
    message >> status;
    if (status == CONFIGURATION_ERROR) {
        string errorMessage;
        message >> errorMessage;
        throw ConfigurationError(errorMessage);
    }
    message >> timeTicks >> passed >> failure;
}

ExecutionInfo::ExecutionInfo(double _timeTicks, bool _passed, string _failure):
        timeTicks(_timeTicks), passed(_passed), failure(move(_failure)) {}

Message ExecutionInfo::toMessage() const {
    return Message::build(FINISHED_SUCCESSFULLY, timeTicks, passed, failure);
}

Test::Test(const TestConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

void Test::setExecuted(const ExecutionInfo& info) {
    if (isExecuted()) {
        throw Bug("Test::setExecuted called twice on the same test.");
    }
    executionInfo = unique_ptr<ExecutionInfo>(new ExecutionInfo(info));
    if (executionInfo->timeTicks > config.timeTicksLimit) {
        executionInfo->passed = false;
        executionInfo->failure = "Execution timed out.";
    }
}

const TestConfig& Test::getConfig() const {
    return config;
}

bool Test::isExecuted() const {
    return executionInfo != nullptr;
}

bool Test::isFailed() const {
    return isExecuted() && !executionInfo->passed;
}

Group* Test::getGroup() const {
    return parentGroup;
}

TestInfo Test::getTestInfo() const {
    return TestInfo(parentGroup->getIndex(),
                    index,
                    config.optional,
                    config.description,
                    executionInfo == nullptr ? false : executionInfo->passed,
                    executionInfo == nullptr ? "" : executionInfo->failure);
}

}
