#include "core/src/driver.hpp"

#include "core/include/kktest_impl/config.hpp"
#include "core/src/box_executor.hpp"
#include "core/src/test_case_registry.hpp"

using namespace std;
using namespace std::placeholders;

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
    instance = new Driver(hooks, smooth, numBoxes);
    instance->hooks.runHooks<Hooks::AFTER_INIT>();
    return instance;
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

Driver::Driver(Hooks hooks, bool smooth, size_t numBoxes):
        hooks(move(hooks)),
        executor(smooth
             ? new    Executor(bind(&Driver::afterTest, this, _1))
             : new BoxExecutor(bind(&Driver::afterTest, this, _1), numBoxes)) {}

void Driver::addGroup(GroupConfig&& config, Executable func) {
    executor->checkIsInactive("group");
    auto group = new Group(move(config),
                           groupStack.back(),
                           ++ currentGroupIndex);
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

void Driver::addTest(TestConfig&& config, Executable func) {
    executor->checkIsInactive("test");
    Group* parentGroup = groupStack.back();
    Test test(move(config), parentGroup, ++ currentTestIndex);
    markTestStarted(parentGroup);
    beforeTest(test);
    executor->execute(move(test), func);
}

void Driver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->addSetUp(func);
}

void Driver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->addTearDown(func);
}

void Driver::beforeTest(const Test& test) {
    hooks.runHooks<Hooks::BEFORE_TEST>(test);
}

void Driver::afterTest(const TestRun& testRun) {
    if (!testRun.isTestOptional()) {
        failedAnyNonOptionalTest |= !testRun.isPassed();
    }
    hooks.runHooks<Hooks::AFTER_TEST>(testRun);
    markTestFinished(testRun.getGroup());
}

void Driver::beforeGroup(Group* group) {
    hooks.runHooks<Hooks::BEFORE_GROUP>(group);
}

void Driver::afterGroup(Group* group) {
    hooks.runHooks<Hooks::AFTER_GROUP>(group);
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

void Driver::markAllTestsStarted(Group* group) {
    groupsPendingFinish.insert(group);
    if (groupsPendingFinish.count(group) && testsInExecution[group] == 0) {
        afterGroup(group);
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

}
