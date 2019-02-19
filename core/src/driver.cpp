#include "core/src/driver.hpp"

#include "core/include/kktest.hpp"
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

int Driver::clean() {
    hooks.runHooks<Hooks::BEFORE_DESTROY>();
    TestCaseRegistry::clean();
    return failedAnyNonOptionalTest ? 1 : 0;
}

void Driver::forceDestroy(const ConfigurationError& error) {
    hooks.runHooks<Hooks::BEFORE_FORCE_DESTROY>(error);
    TestCaseRegistry::clean();
}

void Driver::beforeTestCase(const string& name) {
    auto globalScope = make_shared<Group>(name, nullptr, 0);
    groupStack = {globalScope};
    beforeGroup(globalScope);
}

void Driver::afterTestCase() {
    executor->finalize();
    afterGroup(groupStack.back());
}

Driver::Driver(Hooks hooks, bool smooth, size_t numBoxes):
        hooks(move(hooks)),
        executor(smooth
             ? new    Executor(bind(&Driver::afterTest, this, _1))
             : new BoxExecutor(bind(&Driver::afterTest, this, _1), numBoxes)) {}

void Driver::addGroup(GroupConfig&& config, Executable func) {
    executor->checkIsInactive("group");
    auto group = make_shared<Group>(move(config),
                                    groupStack.back(),
                                    ++ groupIndex);
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
    GroupPtr parentGroup = groupStack.back();
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

void Driver::afterTest(const ExecutedTest& test) {
    failedAnyNonOptionalTest |= (!test.isOptional() && !test.isPassed());
    hooks.runHooks<Hooks::AFTER_TEST>(test);
    markTestFinished(test.getGroup());
}

void Driver::beforeGroup(GroupPtr group) {
    hooks.runHooks<Hooks::BEFORE_GROUP>(group);
}

void Driver::afterGroup(GroupPtr group) {
    hooks.runHooks<Hooks::AFTER_GROUP>(group);
    testsInExecution.erase(group);
    groupsPendingFinish.erase(group);
}

void Driver::markTestStarted(GroupPtr group) {
    while (group != nullptr) {
        testsInExecution[group] += 1;
        group = group->getParentGroup();
    }
}

void Driver::markAllTestsStarted(GroupPtr group) {
    groupsPendingFinish.insert(group);
    if (groupsPendingFinish.count(group) && testsInExecution[group] == 0) {
        afterGroup(group);
    }
}

void Driver::markTestFinished(GroupPtr group) {
    while (group != nullptr) {
        testsInExecution[group] -= 1;
        GroupPtr parentGroup = group->getParentGroup();
        if (groupsPendingFinish.count(group) && testsInExecution[group] == 0) {
            afterGroup(group);
        }
        group = parentGroup;
    }
}

}
