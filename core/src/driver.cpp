#include "core/src/driver.hpp"

#include "core/include/kktest.hpp"
#include "core/src/box_executor.hpp"

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
    return failedAnyNonOptionalTest ? 1 : 0;
}

void Driver::forceDestroy(const ConfigurationError& error) {
    hooks.runHooks<Hooks::BEFORE_FORCE_DESTROY>(error);
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
    group->setStartedAllTests();
    if (group->finishedAllTests()) {
        afterGroup(group);
    }
    groupStack.pop_back();
}

void Driver::addTest(TestConfig&& config, Executable func) {
    executor->checkIsInactive("test");
    GroupPtr parentGroup = groupStack.back();
    Test test(move(config), parentGroup, ++ currentTestIndex);
    parentGroup->addStartedTest();
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
    GroupPtr parentGroup = test.getGroup();
    parentGroup->addFinishedTest();
    while (parentGroup != nullptr) {
        if (parentGroup->finishedAllTests()) {
            afterGroup(parentGroup);
        }
        parentGroup = parentGroup->getParentGroup();
    }
}

void Driver::beforeGroup(GroupPtr group) {
    hooks.runHooks<Hooks::BEFORE_GROUP>(group);
}

void Driver::afterGroup(GroupPtr group) {
    hooks.runHooks<Hooks::AFTER_GROUP>(group);
}

}
