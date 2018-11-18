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

void TestingDriver::addAfterInitHook(AfterInitHook hook) {
    getInstance()->afterInitHooks.push_back(hook);
}

void TestingDriver::addBeforeDestroyHook(BeforeDestroyHook hook) {
    getInstance()->beforeDestroyHooks.push_back(hook);
}

void TestingDriver::addBeforeForceDestroyHook(BeforeForceDestroyHook hook) {
    getInstance()->beforeForceDestroyHooks.push_back(hook);
}

TestingDriver* TestingDriver::instance = nullptr;

TestingDriver* TestingDriver::getInstance() {
    if (instance == nullptr) {
        throw KKTestLibraryImplementationError(
                "TestingDriver::getInstance() called before TestingDriver::init."
        );
    }
    return instance;
}

void TestingDriver::init() {
    if (instance != nullptr) {
        throw KKTestLibraryImplementationError("TestingDriver::init called a twice.");
    }
    instance = new TestingDriver();
    instance->installPlugins();
    addAfterTestHook([](Test* test) {
        if (!test->getConfig().optional) {
            instance->failedAnyNonOptionalTest |= test->isFailed();
        }
        test->getGroup()->markTestFinishedExecution();
    });
    for (const AfterInitHook& hook: instance->afterInitHooks) {
        hook();
    }
}

int TestingDriver::destroy() {
    TestingDriver* driver = getInstance();
    driver->executor->finalize();
    for (const BeforeDestroyHook& hook: driver->beforeDestroyHooks) {
        hook();
    }
    driver->uninstallPlugins();
    int status = driver->failedAnyNonOptionalTest ? 1 : 0;
    delete driver;
    return status;
}

void TestingDriver::forceDestroy(const ConfigurationError& error) {
    TestingDriver* driver = getInstance();
    driver->executor->finalize();
    for (const BeforeForceDestroyHook& hook: driver->beforeForceDestroyHooks) {
        hook(error);
    }
    driver->uninstallPlugins();
    delete driver;
}

TestingDriver::TestingDriver():
        globalScope(new Group(GroupConfig(), nullptr, -1)),
        groupStack({globalScope}),
        executor(new Executor()) {}

TestingDriver::~TestingDriver() {
    delete globalScope;
    if (useImplicitExecutor) {
        delete executor;
    }
}

void TestingDriver::installPlugins() {
    for (int i = 0; i < Plugin::numPlugins; ++ i) {
        auto plugin = Plugin::plugins[i];
        if (plugin->isEnabled()) {
            plugin->install();
        }
    }
}

void TestingDriver::uninstallPlugins() {
    for (int i = 0; i < Plugin::numPlugins; ++ i) {
        Plugin* plugin = Plugin::plugins[i];
        if (plugin->isEnabled()) {
            plugin->uninstall();
        }
    }
}

void TestingDriver::addGroup(const GroupConfig& config, Executable func) {
    executor->checkIsInactive("kkGroup", config.file, config.line);
    auto group = new Group(config, groupStack.back(), ++ currentGroupIndex);
    groupStack.push_back(group);

    beforeGroup(group);
    try {
        func();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const exception& e) {
        throw ConfigurationError("An exception was thrown inside group '" +
            group->getFullDescription() + "': " + e.what());
    } catch(...) {
        throw ConfigurationError("A non-exception object was thrown inside group'" +
            group->getFullDescription() + "'");
    }
    group->markAllTestsStartedExecution([this, group]() {
        afterGroup(group);
    });
    groupStack.pop_back();
}

void TestingDriver::addTest(const TestConfig& config, Executable func) {
    executor->checkIsInactive("kkTest", config.file, config.line);
    Group* parentGroup = groupStack.back();
    auto test = new Test(config, parentGroup, ++ currentTestIndex);
    parentGroup->markTestStartedExecution();
    beforeTest(test);
    executor->execute(test, func, [this, test]() {
        afterTest(test);
    });
}

void TestingDriver::addSetUp(Executable func, const string& file, int line) {
    executor->checkIsInactive("kkSetUp", file, line);
    groupStack.back()->addSetUp(func, file, line);
}

void TestingDriver::addTearDown(Executable func, const string& file, int line) {
    executor->checkIsInactive("kkTearDown", file, line);
    groupStack.back()->addTearDown(func, file, line);
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
