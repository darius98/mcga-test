#include "driver.hpp"
#include "test_case_registry.hpp"

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
    for (const AfterInitHook& hook: instance->afterInitHooks) {
        hook();
    }
    instance->executor->onTestFinished([](Test* test) {
        instance->afterTest(test);
    });
}

int TestingDriver::destroy() {
    TestingDriver* driver = getInstance();
    driver->executor->finalize();
    for (const BeforeDestroyHook& hook: driver->beforeDestroyHooks) {
        hook();
    }
    driver->uninstallPlugins();
    int status = driver->failedAnyNonOptionalTest ? 1 : 0;
    Plugin::clean();
    TestCaseRegistry::clean();
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
    Plugin::clean();
    TestCaseRegistry::clean();
    delete driver;
}

void TestingDriver::beforeTestCase() {
    TestingDriver* driver = getInstance();
    if (driver->globalScope != nullptr) {
        throw KKTestLibraryImplementationError(
            "TestingDriver::beforeTestCase called twice in a row."
        );
    }
    driver->globalScope = new Group(GroupConfig(), nullptr, -1);
    driver->groupStack = {driver->globalScope};
}

void TestingDriver::afterTestCase() {
    TestingDriver* driver = getInstance();
    if (driver->globalScope == nullptr) {
        throw KKTestLibraryImplementationError(
            "TestingDriver::afterTestCase called twice in a row."
        );
    }
    delete driver->globalScope;
    driver->globalScope = nullptr;
    driver->groupStack = {};
}

TestingDriver::TestingDriver(): executor(new Executor()) {}

TestingDriver::~TestingDriver() {
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
    } catch(const ExpectationFailed& e) {
        throw ConfigurationError(
            "Expectation failed in group '" + group->getFullDescription() + "': " + e.what()
        );
    } catch(const exception& e) {
        throw ConfigurationError(
            "Exception thrown in group '" + group->getFullDescription() + "': " + e.what()
        );
    } catch(...) {
        throw ConfigurationError(
            "Non-exception object thrown in group'" + group->getFullDescription() + "'"
        );
    }
    markAllTestsStarted(group);
    groupStack.pop_back();
}

void TestingDriver::addTest(const TestConfig& config, Executable func) {
    executor->checkIsInactive("kkTest", config.file, config.line);
    Group* parentGroup = groupStack.back();
    auto test = new Test(config, parentGroup, ++ currentTestIndex);
    markTestStarted(parentGroup);
    beforeTest(test);
    executor->execute(test, func);
}

void TestingDriver::addSetUp(Executable func, const string& file, int line) {
    executor->checkIsInactive("kkSetUp", file, line);
    groupStack.back()->addSetUp(func, file, line);
}

void TestingDriver::addTearDown(Executable func, const string& file, int line) {
    executor->checkIsInactive("kkTearDown", file, line);
    groupStack.back()->addTearDown(func, file, line);
}

void TestingDriver::beforeTest(Test* test) {
    for (const TestHook& hook: beforeTestHooks) {
        hook(*test);
    }
}

void TestingDriver::afterTest(Test* test) {
    if (!test->getConfig().optional) {
        failedAnyNonOptionalTest |= test->isFailed();
    }
    Group* group = test->getGroup();
    for (const TestHook& hook: afterTestHooks) {
        hook(*test);
    }
    delete test;
    markTestFinished(group);
}

void TestingDriver::beforeGroup(Group* group) {
    for (const GroupHook& hook: beforeGroupHooks) {
        hook(*group);
    }
}

void TestingDriver::afterGroup(Group* group) {
    for (const GroupHook &hook: afterGroupHooks) {
        hook(*group);
    }
    delete group;
    testsInExecutionPerGroup.erase(group);
    groupsWithAllTestsStarted.erase(group);
}

void TestingDriver::markTestStarted(Group* group) {
    if (group == nullptr) {
        return;
    }
    testsInExecutionPerGroup[group] += 1;
    markTestStarted(group->getParentGroup());
}

void TestingDriver::markTestFinished(Group* group) {
    if (group == nullptr) {
        return;
    }
    testsInExecutionPerGroup[group] -= 1;
    Group* parentGroup = group->getParentGroup();
    if (groupsWithAllTestsStarted.count(group) && testsInExecutionPerGroup[group] == 0) {
        afterGroup(group);
    }
    markTestFinished(parentGroup);
}

void TestingDriver::markAllTestsStarted(Group* group) {
    groupsWithAllTestsStarted.insert(group);
    if (groupsWithAllTestsStarted.count(group) && testsInExecutionPerGroup[group] == 0) {
        afterGroup(group);
    }
}

}
