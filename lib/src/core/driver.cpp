#include <fstream>

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
        throw runtime_error("TestingDriver::getInstance() called "
                            "before TestingDriver::init.");
    }
    return instance;
}

void TestingDriver::init(const vector<Plugin*>& plugins) {
    if (instance != nullptr) {
        throw runtime_error("TestingDriver::init called a second time!");
    }
    instance = new TestingDriver(plugins);
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
    int status = driver->globalScope->getNumFailedTests();
    delete driver;
    return status;
}

TestingDriver::TestingDriver(const vector<Plugin*>& plugins):
        Pluginable(plugins),
        globalScope(new Group("", "", 0, nullptr)),
        groupStack({globalScope}),
        executor(new Executor()) {}

TestingDriver::~TestingDriver() {
    delete globalScope;
    if (useImplicitExecutor) {
        delete executor;
    }
}

void TestingDriver::addGroup(string description,
                             string file,
                             int line,
                             Executable func) {
    executor->checkIsInactive("group");
    Group* group = groupStack.back()->addSubGroup(
        move(description), move(file), line
    );
    groupStack.push_back(group);

    beforeGroup(group);
    try {
        func();
    } catch(const exception& e) {
        throw runtime_error("An exception was thrown inside group '" +
            group->getFullDescription() +
            "': " +
            e.what());
    } catch(...) {
        throw runtime_error(
            "A non-exception object was thrown inside group'" +
            group->getFullDescription() + "'");
    }
    afterGroup(group);

    groupStack.pop_back();
}

void TestingDriver::addTest(string description,
                            string file,
                            int line,
                            Executable func) {
    executor->checkIsInactive("test");
    Test* test = groupStack.back()->addTest(
        move(description), move(file), line
    );
    beforeTest(test);
    executor->execute(test, func, [this, test]() {
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
    for (const TestHook& hook: beforeTestHooks) {
        hook(test);
    }
}

void TestingDriver::afterTest(Test* test) const {
    for (const TestHook& hook: afterTestHooks) {
        hook(test);
    }
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
}

}
