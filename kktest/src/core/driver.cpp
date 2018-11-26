#include <EasyFlags.hpp>

#include <core/box_executor.hpp>
#include <core/driver.hpp>
#include <core/test_case_registry.hpp>

using namespace std;

AddArgument(int, flagBoxed)
    .ArgumentType("0|1 ")
    .Name("boxed")
    .Short("b")
    .Description("Run each test in an isolated process (boxed)")
    .DefaultValue(0)
    .ImplicitValue(1);
AddArgument(int, argumentNumBoxes)
    .ArgumentType("int")
    .Name("max_parallel_tests")
    .Description("Maximum number of tests to execute in parallel "
    "(processes to spawn) when running boxed")
    .DefaultValue("1");

namespace kktest {

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
    instance->allowRegisterHooks = true;
    instance->installPlugins();
    instance->allowRegisterHooks = false;
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
    Plugin::clean();
    TestCaseRegistry::clean();
    delete driver;
    return status;
}

void TestingDriver::forceDestroy(const ConfigurationError& error) {
    TestingDriver* driver = getInstance();
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
    driver->executor->finalize();
    delete driver->globalScope;
    driver->globalScope = nullptr;
    driver->groupStack = {};
}

TestingDriver::TestingDriver() {
    if (flagBoxed) {
        executor = new BoxExecutor((size_t)max(argumentNumBoxes, 1));
    } else {
        executor = new Executor();
    }
    executor->onTestFinished([this](Test* test) {
        afterTest(test);
    });
}

TestingDriver::~TestingDriver() {
    delete executor;
}

void TestingDriver::installPlugins() {
    if (Plugin::plugins == nullptr) {
        return;
    }
    for (Plugin* plugin: (*Plugin::plugins)) {
        if (plugin->isEnabled()) {
            plugin->install();
        }
    }
}

void TestingDriver::uninstallPlugins() {
    if (Plugin::plugins == nullptr) {
        return;
    }
    for (Plugin* plugin: (*Plugin::plugins)) {
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
    TestInfo info = test->getTestInfo();
    for (const TestHook& hook: beforeTestHooks) {
        hook(info);
    }
}

void TestingDriver::afterTest(Test* test) {
    if (!test->getConfig().optional) {
        failedAnyNonOptionalTest |= test->isFailed();
    }
    Group* group = test->getGroup();
    TestInfo info = test->getTestInfo();
    for (const TestHook& hook: afterTestHooks) {
        hook(info);
    }
    delete test;
    markTestFinished(group);
}

void TestingDriver::beforeGroup(Group* group) {
    GroupInfo info = group->getGroupInfo();
    for (const GroupHook& hook: beforeGroupHooks) {
        hook(info);
    }
}

void TestingDriver::afterGroup(Group* group) {
    GroupInfo info = group->getGroupInfo();
    for (const GroupHook &hook: afterGroupHooks) {
        hook(info);
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
