#include <utility>

#include <kktest_impl/config.hpp>
#include "box_executor.hpp"
#include "driver.hpp"
#include "test_case_registry.hpp"

using std::exception;
using std::move;

namespace kktest {

TestingDriver* TestingDriver::instance = nullptr;

TestingDriver* TestingDriver::getInstance() {
    if (instance == nullptr) {
        throw KKTestLibraryImplementationError(
                "TestingDriver::getInstance() called before TestingDriver::init.");
    }
    return instance;
}

TestingDriver* TestingDriver::init(const TestingDriverHooks& hooks,
                                   bool flagBoxed,
                                   int argumentNumBoxes) {
    if (instance != nullptr) {
        throw KKTestLibraryImplementationError("TestingDriver::init called a twice.");
    }
    return new TestingDriver(hooks, flagBoxed, argumentNumBoxes);
}

int TestingDriver::destroy() {
    executor->finalize();
    hookManager.runHooks<TestingDriverHooks::BEFORE_DESTROY>();
    int status = failedAnyNonOptionalTest ? 1 : 0;
    TestCaseRegistry::clean();
    delete this;
    return status;
}

void TestingDriver::forceDestroy(const ConfigurationError& error) {
    hookManager.runHooks<TestingDriverHooks::BEFORE_FORCE_DESTROY>(error);
    TestCaseRegistry::clean();
    delete this;
}

void TestingDriver::beforeTestCase(const String& name) {
    if (globalScope != nullptr) {
        throw KKTestLibraryImplementationError(
            "TestingDriver::beforeTestCase called twice in a row.");
    }
    globalScope = new Group(groupConfig(_.description = name), nullptr, 0);
    groupStack = {globalScope};
    beforeGroup(globalScope);
}

void TestingDriver::afterTestCase() {
    if (globalScope == nullptr) {
        throw KKTestLibraryImplementationError(
            "TestingDriver::afterTestCase called twice in a row.");
    }
    executor->finalize();
    afterGroup(globalScope);
    globalScope = nullptr;
    groupStack = {};
}

TestingDriver::TestingDriver(TestingDriverHooks hooks, bool flagBoxed, int argumentNumBoxes):
        hookManager(move(hooks)) {
    instance = this;
    if (flagBoxed) {
        executor = new BoxExecutor((size_t)argumentNumBoxes);
    } else {
        executor = new Executor();
    }
    executor->onTestFinished([this](Test* test) {
        afterTest(test);
    });
    hookManager.runHooks<TestingDriverHooks::AFTER_INIT>();
}

TestingDriver::~TestingDriver() {
    delete executor;
}

void TestingDriver::addGroup(const GroupConfig& config, Executable func) {
    executor->checkIsInactive("group");
    auto group = new Group(config, groupStack.back(), ++currentGroupIndex);
    groupStack.push_back(group);

    beforeGroup(group);
    try {
        func();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& e) {
        throw ConfigurationError(
            "Expectation failed in group \"" + group->getDescription() + "\": " + e.what());
    } catch(const exception& e) {
        throw ConfigurationError(
            "Exception thrown in group \"" + group->getDescription() + "\": " + e.what());
    } catch(...) {
        throw ConfigurationError(
            "Non-exception object thrown in group \"" + group->getDescription() + "\".");
    }
    markAllTestsStarted(group);
    groupStack.pop_back();
}

void TestingDriver::addTest(const TestConfig& config, Executable func) {
    executor->checkIsInactive("test");
    Group* parentGroup = groupStack.back();
    auto test = new Test(config, parentGroup, ++currentTestIndex);
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

}  // namespace kktest
