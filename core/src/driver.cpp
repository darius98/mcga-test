#include "core/src/driver.hpp"

#include "core/include/kktest_impl/config.hpp"
#include "core/src/box_executor.hpp"
#include "core/src/test_case_registry.hpp"

using namespace std;

namespace kktest {

Driver* Driver::instance = nullptr;

Driver* Driver::getInstance() {
    if (instance == nullptr) {
        throw Bug("Driver: getInstance() called before init().");
    }
    return instance;
}

Driver* Driver::init(const Hooks& hooks, bool smooth, size_t numBoxes) {
    if (instance != nullptr) {
        throw Bug("Driver: init() called a twice.");
    }
    return new Driver(hooks, smooth, numBoxes);
}

int Driver::destroy() {
    executor->finalize();
    hooks.runHooks<Hooks::BEFORE_DESTROY>();
    int status = failedAnyNonOptionalTest ? 1 : 0;
    TestCaseRegistry::clean();
    delete this;
    return status;
}

void Driver::forceDestroy(const ConfigurationError& error) {
    hooks.runHooks<Hooks::BEFORE_FORCE_DESTROY>(error);
    TestCaseRegistry::clean();
    delete this;
}

void Driver::beforeTestCase(const string& name) {
    if (globalScope != nullptr) {
        throw Bug("Driver: beforeTestCase() called twice in a row.");
    }
    globalScope = new Group(name, nullptr, 0);
    groupStack = {globalScope};
    beforeGroup(globalScope);
}

void Driver::afterTestCase() {
    if (globalScope == nullptr) {
        throw Bug("Driver: afterTestCase() called twice in a row.");
    }
    executor->finalize();
    afterGroup(globalScope);
    globalScope = nullptr;
    groupStack = {};
}

Driver::Driver(Hooks hooks, bool smooth, size_t numBoxes): hooks(move(hooks)) {
    instance = this;
    auto onTestFinishedCallback = [this](Test* test, TestExecutionInfo info) {
        test->setExecuted(info);
        afterTest(test);
    };
    if (smooth) {
        executor = new Executor(onTestFinishedCallback);
    } else {
        executor = new BoxExecutor(onTestFinishedCallback, numBoxes);
    }
    hooks.runHooks<Hooks::AFTER_INIT>();
}

Driver::~Driver() {
    delete executor;
}

void Driver::addGroup(const GroupConfig& config, Executable func) {
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
            "Expectation failed in group \"" + group->getDescription() + "\""
            ": " + e.what());
    } catch(const exception& e) {
        throw ConfigurationError(
            "Exception thrown in group \"" + group->getDescription() + "\""
            ": " + e.what());
    } catch(...) {
        throw ConfigurationError(
            "Non-exception thrown in group \"" + group->getDescription() + "\""
            ".");
    }
    markAllTestsStarted(group);
    groupStack.pop_back();
}

void Driver::addTest(const TestConfig& config, Executable func) {
    executor->checkIsInactive("test");
    Group* parentGroup = groupStack.back();
    auto test = new Test(config, parentGroup, ++currentTestIndex);
    markTestStarted(parentGroup);
    beforeTest(test);
    executor->execute(test, func);
}

void Driver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->addSetUp(func);
}

void Driver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->addTearDown(func);
}

void Driver::beforeTest(Test* test) {
    hooks.runHooks<Hooks::BEFORE_TEST>(test->getTestInfo());
}

void Driver::afterTest(Test* test) {
    if (!test->getConfig().optional) {
        failedAnyNonOptionalTest |= test->isFailed();
    }
    Group* group = test->getGroup();
    hooks.runHooks<Hooks::AFTER_TEST>(test->getTestInfo());
    delete test;
    markTestFinished(group);
}

void Driver::beforeGroup(Group* group) {
    hooks.runHooks<Hooks::BEFORE_GROUP>(group->getGroupInfo());
}

void Driver::afterGroup(Group* group) {
    hooks.runHooks<Hooks::AFTER_GROUP>(group->getGroupInfo());
    delete group;
    testsInExecution.erase(group);
    groupsPendingFinish.erase(group);
}

void Driver::markTestStarted(Group* group) {
    while (group != nullptr) {
        testsInExecution[group] += 1;
        group = group->getParentGroup();
    }
}

void Driver::markTestFinished(Group* group) {
    while (group != nullptr) {
        testsInExecution[group] -= 1;
        Group* parentGroup = group->getParentGroup();
        if (groupsPendingFinish.count(group) && testsInExecution[group] == 0) {
            afterGroup(group);
        }
        group = parentGroup;
    }
}

void Driver::markAllTestsStarted(Group* group) {
    groupsPendingFinish.insert(group);
    if (groupsPendingFinish.count(group) && testsInExecution[group] == 0) {
        afterGroup(group);
    }
}

}
