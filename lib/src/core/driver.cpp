#include "driver.hpp"

using namespace std;


namespace kktest {

void TestingDriver::setExecutor(Executor* executor) {
    if (getInstance()->useImplicitExecutor) {
        delete getInstance()->executor;
        getInstance()->useImplicitExecutor = false;
    }
    getInstance()->executor = executor;
}

void TestingDriver::addBeforeTestHook(TestHook hook) {
    getInstance()->beforeTestHooks.emplace_back(move(hook));
}

void TestingDriver::addAfterTestHook(TestHook hook) {
    getInstance()->afterTestHooks.emplace_back(move(hook));
}

void TestingDriver::addBeforeGroupHook(GroupHook hook) {
    getInstance()->beforeGroupHooks.emplace_back(move(hook));
}

void TestingDriver::addAfterGroupHook(GroupHook hook) {
    getInstance()->afterGroupHooks.emplace_back(move(hook));
}

void TestingDriver::addAfterInitHook(CopyableExecutable hook) {
    getInstance()->afterInitHooks.push_back(hook);
}

void TestingDriver::addBeforeDestroyHook(CopyableExecutable hook) {
    getInstance()->beforeDestroyHooks.push_back(hook);
}

TestingDriver* TestingDriver::instance = nullptr;

TestingDriver* TestingDriver::getInstance() {
    if (instance == nullptr) {
        throw runtime_error("TestingDriver::getInstance() called before TestingDriver::init.");
    }
    return instance;
}

void TestingDriver::init() {
    if (instance != nullptr) {
        throw runtime_error("TestingDriver::init called a second time!");
    }
    instance = new TestingDriver();
    instance->installPlugins();
    for (Executable hook: instance->afterInitHooks) {
        hook();
    }
}

int TestingDriver::destroy() {
    TestingDriver* driver = getInstance();
    driver->executor->finalize();
    for (Executable hook: driver->beforeDestroyHooks) {
        hook();
    }
    int status = driver->failedAnyNonOptionalTest ? 1 : 0;
    delete driver;
    return status;
}

TestingDriver::TestingDriver():
        Pluginable("kktest"),
        globalScope(new Group(GroupConfig(), nullptr, -1)),
        groupStack({globalScope}),
        executor(new Executor()) {}

TestingDriver::~TestingDriver() {
    delete globalScope;
    if (useImplicitExecutor) {
        delete executor;
    }
}

void TestingDriver::addGroup(const GroupConfig& config, Executable func) {
    executor->checkIsInactive("group");
    Group* group = new Group(config, groupStack.back(), ++ currentGroupIndex);
    groupStack.push_back(group);

    beforeGroup(group);
    try {
        func();
    } catch(const exception& e) {
        throw runtime_error("An exception was thrown inside group '" +
            group->getFullDescription() + "': " + e.what());
    } catch(...) {
        throw runtime_error(
            "A non-exception object was thrown inside group'" +
            group->getFullDescription() + "'");
    }
    group->markAllTestsStartedExecution([this, group]() {
        afterGroup(group);
    });
    groupStack.pop_back();
}

void TestingDriver::addTest(const TestConfig& config, Executable func) {
    executor->checkIsInactive("test");
    Test* test = new Test(config, groupStack.back(), ++ currentTestIndex);
    beforeTest(test);
    executor->execute(test, func, [this, test]() {
        if (!test->getConfig().optional) {
            failedAnyNonOptionalTest |= test->isFailed();
        }
        afterTest(test);
    });
}

void TestingDriver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->addSetUp(func);
}

void TestingDriver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->addTearDown(func);
}

void TestingDriver::beforeTest(Test* test) const {
    test->getParentGroup()->markTestStartedExecution();
    for (const TestHook& hook: beforeTestHooks) {
        hook(test);
    }
}

void TestingDriver::afterTest(Test* test) const {
    for (const TestHook& hook: afterTestHooks) {
        hook(test);
    }
    test->getParentGroup()->markTestFinishedExecution();
    delete test;
}

void TestingDriver::beforeGroup(Group* group) const {
    for (const GroupHook& hook: beforeGroupHooks) {
        hook(group);
    }
}

void TestingDriver::afterGroup(Group* group) const {
    for (const GroupHook& hook: afterGroupHooks) {
        hook(group);
    }
    delete group;
}

}
