#include "kktest/core/driver.hpp"

#include <thread>

#include "kktest/core/include/kktest.hpp"
#include "kktest/core/box_executor.hpp"

using namespace std;

namespace kktest {

Driver* Driver::Instance() {
    return instance;
}

Driver* Driver::Init(const HooksManager& api,
                     ExecutorType executorType,
                     size_t numBoxes) {
    Executor* executor = nullptr;
    switch (executorType) {
        case SMOOTH_EXECUTOR: executor = new Executor(); break;
        case BOXED_EXECUTOR: executor = new BoxExecutor(numBoxes); break;
    }
    instance = new Driver(api, executor);
    instance->runHooks<AFTER_INIT>();
    return instance;
}

Driver::Driver(HooksManager hooksManager, Executor* _executor):
        HooksManager(move(hooksManager)), executor(_executor) {
    testingThreadId = hash<thread::id>()(this_thread::get_id());
    executor->setOnTestFinishedCallback([this](const ExecutedTest& test) {
        afterTest(test);
    });
    executor->setOnWarningCallback([this](const Warning& warning) {
        runHooks<ON_WARNING>(warning);
    });
}

void Driver::addGroup(GroupConfig config, const Executable& body) {
    if (executor->isActive()) {
        emitWarning("Called group() inside a "
                    + executor->stateAsString() + "(). Ignoring.");
        return;
    }
    if (testingThreadId != hash<thread::id>()(this_thread::get_id())) {
        emitWarning("Called group() from a different thread than the main "
                    "testing thread. Ignoring.");
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
        emitWarning("Called test() inside a "
                    + executor->stateAsString() + "(). Ignoring.");
        return;
    }
    if (testingThreadId != hash<thread::id>()(this_thread::get_id())) {
        emitWarning("Called test() from a different thread than the main "
                    "testing thread. Ignoring.");
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
        emitWarning("Called setUp() inside a "
                    + executor->stateAsString() + "(). Ignoring.");
        return;
    }
    if (testingThreadId != hash<thread::id>()(this_thread::get_id())) {
        emitWarning("Called setUp() from a different thread than the main "
                    "testing thread. Ignoring.");
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
    if (executor->isActive()) {
        emitWarning("Called tearDown() inside a "
                    + executor->stateAsString() + "(). Ignoring.");
        return;
    }
    if (testingThreadId != hash<thread::id>()(this_thread::get_id())) {
        emitWarning("Called tearDown() from a different thread than the main "
                    "testing thread. Ignoring.");
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
        emitWarning("Called fail() with message '" + failure + "' "
                    "outside test()/setUp()/tearDown(). Ignoring.");
        return;
    }
    executor->addFailure(failure);
}

void Driver::clean() {
    executor->finalize();
    runHooks<BEFORE_DESTROY>();
}

void Driver::emitWarning(const string& message) {
    if (executor->isActive()) {
        executor->handleWarning(message);
    } else {
        runHooks<ON_WARNING>(Warning(message, groupStack.back()->getId()));
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
