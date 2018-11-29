#include <EasyFlags.hpp>

#include "box_executor.hpp"
#include "driver.hpp"
#include "test_case_registry.hpp"

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

TestingDriver* TestingDriver::init(const vector<Plugin*>& plugins) {
    if (instance != nullptr) {
        throw KKTestLibraryImplementationError("TestingDriver::init called a twice.");
    }
    return new TestingDriver(plugins);
}

int TestingDriver::destroy() {
    executor->finalize();
    hookManager.runHooks<TestingDriverHooks::BEFORE_DESTROY>();
    uninstallPlugins();
    int status = failedAnyNonOptionalTest ? 1 : 0;
    TestCaseRegistry::clean();
    delete this;
    return status;
}

void TestingDriver::forceDestroy(const ConfigurationError& error) {
    hookManager.runHooks<TestingDriverHooks::BEFORE_FORCE_DESTROY>(error);
    uninstallPlugins();
    TestCaseRegistry::clean();
    delete this;
}

void TestingDriver::beforeTestCase() {
    if (globalScope != nullptr) {
        throw KKTestLibraryImplementationError(
            "TestingDriver::beforeTestCase called twice in a row."
        );
    }
    globalScope = new Group(GroupConfig(), nullptr, -1);
    groupStack = {globalScope};
}

void TestingDriver::afterTestCase() {
    if (globalScope == nullptr) {
        throw KKTestLibraryImplementationError(
            "TestingDriver::afterTestCase called twice in a row."
        );
    }
    executor->finalize();
    delete globalScope;
    globalScope = nullptr;
    groupStack = {};
}

TestingDriver::TestingDriver(const vector<Plugin*>& _plugins): plugins(_plugins) {
    instance = this;
    if (flagBoxed) {
        executor = new BoxExecutor((size_t)max(argumentNumBoxes, 1));
    } else {
        executor = new Executor();
    }
    executor->onTestFinished([this](Test* test) {
        afterTest(test);
    });
    allowRegisterHooks = true;
    installPlugins();
    allowRegisterHooks = false;
    hookManager.runHooks<TestingDriverHooks::AFTER_INIT>();
}

TestingDriver::~TestingDriver() {
    delete executor;
}

void TestingDriver::installPlugins() {
    for (Plugin* plugin: plugins) {
        plugin->install();
    }
}

void TestingDriver::uninstallPlugins() {
    for (Plugin* plugin: plugins) {
        plugin->uninstall();
    }
}

void TestingDriver::addGroup(const GroupConfig& config, Executable func) {
    executor->checkIsInactive("group");
    auto group = new Group(config, groupStack.back(), ++ currentGroupIndex);
    groupStack.push_back(group);

    beforeGroup(group);
    try {
        func();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& e) {
        throw ConfigurationError(
            "Expectation failed in group \"" + group->getConfig().description + "\": " + e.what()
        );
    } catch(const exception& e) {
        throw ConfigurationError(
            "Exception thrown in group \"" + group->getConfig().description + "\": " + e.what()
        );
    } catch(...) {
        throw ConfigurationError(
            "Non-exception object thrown in group \"" + group->getConfig().description + "\"."
        );
    }
    markAllTestsStarted(group);
    groupStack.pop_back();
}

void TestingDriver::addTest(const TestConfig& config, Executable func) {
    executor->checkIsInactive("test");
    Group* parentGroup = groupStack.back();
    auto test = new Test(config, parentGroup, ++ currentTestIndex);
    markTestStarted(parentGroup);
    beforeTest(test);
    executor->execute(test, func);
}

void TestingDriver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->addSetUp(func);
}

void TestingDriver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->addTearDown(func);
}

void TestingDriver::beforeTest(Test* test) {
    hookManager.runHooks<TestingDriverHooks::BEFORE_TEST>(test->getTestInfo());
}

void TestingDriver::afterTest(Test* test) {
    if (!test->getConfig().optional) {
        failedAnyNonOptionalTest |= test->isFailed();
    }
    Group* group = test->getGroup();
    hookManager.runHooks<TestingDriverHooks::AFTER_TEST>(test->getTestInfo());
    delete test;
    markTestFinished(group);
}

void TestingDriver::beforeGroup(Group* group) {
    hookManager.runHooks<TestingDriverHooks::BEFORE_GROUP>(group->getGroupInfo());
}

void TestingDriver::afterGroup(Group* group) {
    hookManager.runHooks<TestingDriverHooks::AFTER_GROUP>(group->getGroupInfo());
    delete group;
    testsInExecutionPerGroup.erase(group);
    groupsWithAllTestsStarted.erase(group);
}

void TestingDriver::markTestStarted(Group* group) {
    while (group != nullptr) {
        testsInExecutionPerGroup[group] += 1;
        group = group->getParentGroup();
    }
}

void TestingDriver::markTestFinished(Group* group) {
    while (group != nullptr) {
        testsInExecutionPerGroup[group] -= 1;
        Group* parentGroup = group->getParentGroup();
        if (groupsWithAllTestsStarted.count(group) && testsInExecutionPerGroup[group] == 0) {
            afterGroup(group);
        }
        group = parentGroup;
    }
}

void TestingDriver::markAllTestsStarted(Group* group) {
    groupsWithAllTestsStarted.insert(group);
    if (groupsWithAllTestsStarted.count(group) && testsInExecutionPerGroup[group] == 0) {
        afterGroup(group);
    }
}

}
