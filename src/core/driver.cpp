#include "driver.hpp"

#include <thread>

#include "box_executor.hpp"
#include "mcga/test.hpp"

using std::exception;
using std::hash;
using std::make_shared;
using std::move;
using std::string;
using std::thread;

namespace mcga::test {

Driver* Driver::Instance() {
    return instance;
}

void Driver::Init(Driver* _instance) {
    instance = _instance;
}

void Driver::Clean() {
    instance->executor->finalize();
    instance = nullptr;
}

Driver::Driver(Executor* executor): executor(executor) {
}

Executor::Type Driver::getExecutorType() const {
    return executor->getType();
}

void Driver::addGroup(GroupConfig config, const Executable& body) {
    if (!checkMainThreadAndInactive("group")) {
        return;
    }

    ++currentGroupId;
    GroupPtr parentGroup = groupStack.empty() ? nullptr : groupStack.back();
    GroupPtr group
      = make_shared<Group>(move(config), parentGroup, currentGroupId);

    groupStack.push_back(group);
    try {
        body();
    } catch (const exception& e) {
        emitWarning("Exception thrown in group \"" + group->getDescription()
                    + "\": " + e.what()
                    + ". Unable to execute remainder of tests in this group.");
    } catch (...) {
        emitWarning(
          "Non-exception thrown in group \"" + group->getDescription()
          + "\". Unable to execute remainder of tests in this group.");
    }
    groupStack.pop_back();
}

void Driver::addTest(TestConfig config, Executable body) {
    if (!checkMainThreadAndInactive("test")) {
        return;
    }
    GroupPtr parentGroup = groupStack.back();
    executor->execute(
      Test(move(config), move(body), parentGroup, ++currentTestId));
}

void Driver::addSetUp(Executable func) {
    if (!checkMainThreadAndInactive("setUp")) {
        return;
    }
    const auto& group = groupStack.back();
    if (group->hasSetUp()) {
        emitWarning("setUp() called, but a setUp for group \""
                    + group->getDescription() + "\" already exists. Ignoring.");
        return;
    }
    group->addSetUp(move(func));
}

void Driver::addTearDown(Executable func) {
    if (!checkMainThreadAndInactive("tearDown")) {
        return;
    }
    const auto& group = groupStack.back();
    if (group->hasTearDown()) {
        emitWarning("tearDown() called, but a tearDown for group \""
                    + group->getDescription() + "\" already exists. Ignoring.");
        return;
    }
    group->addTearDown(move(func));
}

void Driver::addFailure(const string& failure) {
    if (!executor->isActive()) {
        emitWarning("Called fail() with message '" + failure
                    + "' outside a test execution. Ignoring.");
        return;
    }
    executor->addFailure(failure);
}

void Driver::emitWarning(const string& message) {
    executor->emitWarning(message, groupStack.back()->getId());
}

bool Driver::checkMainThreadAndInactive(const string& method) {
    if (executor->isActive()) {
        emitWarning("Called " + method + "() inside a "
                    + executor->stateAsString() + "(). Ignoring.");
        return false;
    }
    if (testingThreadId != hash<thread::id>()(std::this_thread::get_id())) {
        emitWarning("Called " + method
                    + "() from a different thread than the main testing "
                      "thread. Ignoring.");
        return false;
    }
    return true;
}

}  // namespace mcga::test
