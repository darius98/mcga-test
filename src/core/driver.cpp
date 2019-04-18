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
    instance->hooks->runHooks<HooksManager::AFTER_INIT>();
}

void Driver::Clean() {
    instance->executor->finalize();
    instance = nullptr;
}

Driver::Driver(HooksManager* hooks, Executor* executor)
        : hooks(hooks), executor(executor) {
    executor->setOnTestFinishedCallback(
      [this](const ExecutedTest& test) { afterTest(test); });
    executor->setOnWarningCallback(
      [this](const Warning& warning) { onWarning(warning); });
}

Executor::Type Driver::getExecutorType() const {
    return executor->getType();
}

void Driver::addGroup(GroupConfig config, const Executable& body) {
    if (!checkMainThreadAndInactive("group")) {
        return;
    }

    ++currentGroupId;
    auto parentGroup = groupStack.empty() ? nullptr : groupStack.back();
    auto group = make_shared<Group>(move(config), parentGroup, currentGroupId);

    groupStack.push_back(group);
    beforeGroup(group);
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
    group->setStartedAllTests();
    if (group->finishedAllTests()) {
        afterGroup(group);
    }
    groupStack.pop_back();
}

void Driver::addTest(TestConfig config, Executable body) {
    if (!checkMainThreadAndInactive("test")) {
        return;
    }
    GroupPtr parentGroup = groupStack.back();
    Test test(move(config), move(body), parentGroup, ++currentTestId);
    parentGroup->addStartedTest();
    beforeTest(test);
    executor->execute(move(test));
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
    if (executor->isActive()) {
        executor->emitWarning(message);
    } else {
        onWarning(Warning(message, groupStack.back()->getId()));
    }
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

void Driver::onWarning(const Warning& warning) {
    hooks->runHooks<HooksManager::ON_WARNING>(warning);
}

void Driver::beforeTest(const Test& test) {
    hooks->runHooks<HooksManager::BEFORE_TEST>(test);
}

void Driver::afterTest(const ExecutedTest& test) {
    hooks->runHooks<HooksManager::AFTER_TEST>(test);
    GroupPtr parentGroup = test.getGroup();
    parentGroup->addFinishedTest();
    while (parentGroup != nullptr) {
        if (parentGroup->finishedAllTests()) {
            afterGroup(parentGroup);
        }
        parentGroup = parentGroup->getParentGroup();
    }
}

void Driver::beforeGroup(const GroupPtr& group) {
    hooks->runHooks<HooksManager::BEFORE_GROUP>(group);
}

void Driver::afterGroup(const GroupPtr& group) {
    hooks->runHooks<HooksManager::AFTER_GROUP>(group);
}

}  // namespace mcga::test
