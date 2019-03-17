#include "kktest/core/driver.hpp"

#include <iostream>

#include "kktest/core/include/kktest.hpp"
#include "kktest/core/box_executor.hpp"

using namespace std;
using namespace std::placeholders;

namespace kktest {

Driver* Driver::Instance() {
    return instance;
}

Driver* Driver::Init(const HooksManager& api,
                     ExecutorType executorType,
                     size_t numBoxes) {
    Executor* executor;
    switch (executorType) {
        case SMOOTH_EXECUTOR: executor = new Executor(); break;
        case BOXED_EXECUTOR: executor = new BoxExecutor(numBoxes); break;
    }
    instance = new Driver(api, executor);
    instance->runHooks<AFTER_INIT>();
    return instance;
}

void Driver::clean() {
    executor->finalize();
    runHooks<BEFORE_DESTROY>();
}

void Driver::addGroup(GroupConfig config, const Executable& body) {
    if (executor->isActive()) {
        emitWarning("Called kktest::group() inside a kktest::"
                    + executor->stateAsString()
                    + "(). Ignoring...");
        return;
    }

    ++ currentGroupId;
    auto parentGroup = groupStack.empty() ? nullptr : groupStack.back();
    auto group = make_shared<Group>(move(config), parentGroup, currentGroupId);

    groupStack.push_back(group);
    beforeGroup(group);
    try {
        body();
    } catch(const exception& e) {
        emitWarning("Exception thrown in group "
                    "\"" + group->getDescription() + "\": " + e.what()
                    + ". Unable to execute remainder of tests in this group.");
    } catch(...) {
        emitWarning("Non-exception thrown in group "
                    "\"" + group->getDescription() + "\". Unable to execute "
                    "remainder of tests in this group.");
    }
    group->setStartedAllTests();
    if (group->finishedAllTests()) {
        afterGroup(group);
    }
    groupStack.pop_back();
}

void Driver::addTest(TestConfig config, Executable body) {
    if (executor->isActive()) {
        emitWarning("Called kktest::test() inside a kktest::"
                    + executor->stateAsString()
                    + "(). Ignoring...");
        return;
    }
    GroupPtr parentGroup = groupStack.back();
    Test test(move(config), move(body), parentGroup, ++ currentTestId);
    parentGroup->addStartedTest();
    beforeTest(test);
    executor->execute(move(test));
}

void Driver::addSetUp(Executable func) {
    if (executor->isActive()) {
        emitWarning("Called kktest::setUp() inside a kktest::"
                    + executor->stateAsString()
                    + "(). Ignoring...");
        return;
    }
    if (groupStack.back()->hasSetUp()) {
        emitWarning("kktest::setUp() called, but a setUp for group \""
                    + groupStack.back()->getDescription()
                    + "\" already exists. Ignoring...");
        return;
    }
    groupStack.back()->addSetUp(move(func));
}

void Driver::addTearDown(Executable func) {
    if (executor->isActive()) {
        emitWarning("Called kktest::tearDown() inside a kktest::"
                    + executor->stateAsString()
                    + "(). Ignoring...");
        return;
    }
    if (groupStack.back()->hasTearDown()) {
        emitWarning("kktest::tearDown() called, but a tearDown for group \""
                    + groupStack.back()->getDescription()
                    + "\" already exists. Ignoring...");
        return;
    }
    groupStack.back()->addTearDown(move(func));
}

void Driver::addFailure(const string& failure) {
    if (!executor->isActive()) {
        emitWarning("Called kktest::fail() outside "
                    "kktest::test()/kktest::setUp()/kktest::tearDown()."
                    "Ignoring...");
        return;
    }
    throw ExpectationFailed(failure);
}

Driver::Driver(HooksManager hooksManager, Executor* _executor):
        HooksManager(move(hooksManager)), executor(_executor) {
    executor->setOnTestFinishedCallback([this](const ExecutedTest& test) {
        afterTest(test);
    });
    executor->setOnWarningCallback([this](const string& message) {
        runHooks<ON_WARNING>(message);
    });
}

void Driver::emitWarning(const string& message) {
    if (executor->isActive()) {
        executor->handleWarning(message);
    } else {
        runHooks<ON_WARNING>(message);
    }
}

void Driver::beforeTest(const Test& test) {
    runHooks<BEFORE_TEST>(test);
}

void Driver::afterTest(const ExecutedTest& test) {
    runHooks<AFTER_TEST>(test);
    GroupPtr parentGroup = test.getGroup();
    parentGroup->addFinishedTest();
    while (parentGroup != nullptr) {
        if (parentGroup->finishedAllTests()) {
            afterGroup(parentGroup);
        }
        parentGroup = parentGroup->getParentGroup();
    }
}

void Driver::beforeGroup(GroupPtr group) {
    runHooks<BEFORE_GROUP>(group);
}

void Driver::afterGroup(GroupPtr group) {
    runHooks<AFTER_GROUP>(group);
}

}
