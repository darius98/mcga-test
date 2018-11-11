#include "group.hpp"
#include "test.hpp"

using namespace std;


namespace kktest {

Test::Test(const TestConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

void Test::setExecuted(double _executionTimeTicks) {
    if (isExecuted()) {
        throw runtime_error("Test::setExecuted called twice on the same test!");
    }
    executed = true;
    executionTimeTicks = _executionTimeTicks;
}

void Test::setFailure(const string& message, bool force) {
    if (passed || force) {
        passed = false;
        failureMessage = message;
    }
}

const TestConfig& Test::getConfig() const {
    return config;
}

int Test::getIndex() const {
    return index;
}

bool Test::isExecuted() const {
    return executed;
}

bool Test::isFailed() const {
    return isExecuted() && !isPassed();
}

bool Test::isPassed() const {
    return isExecuted() && passed;
}

string Test::getFailureMessage() const {
    return isFailed() ? failureMessage : "";
}

double Test::getExecutionTimeTicks() const {
    return executionTimeTicks;
}

string Test::getDescriptionPrefix() const {
    string prefix = config.file + ":" + to_string(config.line) + "::";
    if (!parentGroup->isGlobalScope()) {
        prefix += parentGroup->getFullDescription() + "::";
    }
    return prefix;
}

void Test::writeBytes(BytesConsumer& consumer) const {
    consumer
        << parentGroup->getIndex()
        << index
        << config.line
        << config.file.size()
        << config.file
        << config.optional
        << config.description.size()
        << config.description
        << isPassed()
        << getFailureMessage().size()
        << getFailureMessage();
}

Group* Test::getParentGroup() const {
    return parentGroup;
}

}
