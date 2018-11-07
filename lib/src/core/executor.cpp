#include <iostream>

#include <EasyFlags.hpp>

#include "executor.hpp"

using namespace std;

namespace kktest {

void Executor::copyHooks(Executor* other) {
    for (const TestHook& hook: other->beforeTestHooks) {
        addBeforeTestHook(hook);
    }
    for (const TestHook& hook: other->afterTestHooks) {
        addAfterTestHook(hook);
    }
    for (const GroupHook& hook: other->beforeGroupHooks) {
        addBeforeGroupHook(hook);
    }
    for (const GroupHook& hook: other->afterGroupHooks) {
        addAfterGroupHook(hook);
    }
}

void Executor::executeTest(Test* test, Executable func) {
    beforeTest(test);
    execute(test, func);
}

void Executor::addBeforeTestHook(TestHook hook) {
    beforeTestHooks.push_back(hook);
}

void Executor::addAfterTestHook(TestHook hook) {
    afterTestHooks.push_back(hook);
}

void Executor::addBeforeGroupHook(GroupHook hook) {
    beforeGroupHooks.push_back(hook);
}

void Executor::addAfterGroupHook(GroupHook hook) {
    afterGroupHooks.push_back(hook);
}

void Executor::beforeTest(Test* test) const {
    for (const TestHook& hook: beforeTestHooks) {
        hook(test);
    }
}

void Executor::afterTest(Test* test) const {
    for (const TestHook& hook: afterTestHooks) {
        hook(test);
    }
}

void Executor::beforeGroup(Group* group) const {
    for (const GroupHook& hook: beforeGroupHooks) {
        hook(group);
    }
}

void Executor::afterGroup(Group* group) const {
    for (const GroupHook& hook: afterGroupHooks) {
        hook(group);
    }
}

bool Executor::isDuringTest() const {
    return state == TEST;
}

void Executor::checkIsInactive(const string& methodName) const {
    if (state == TEST) {
        throw runtime_error("Cannot call " + methodName + " within test!");
    }
    if (state == SET_UP) {
        throw runtime_error("Cannot call " + methodName + " within setUp!");
    }
    if (state == TEAR_DOWN) {
        throw runtime_error("Cannot call " + methodName + " within tearDown!");
    }
}

void Executor::finalize() {}

void Executor::execute(Test* test, Executable func) {
    run(test, func);
    afterTest(test);
}

void Executor::run(Test* test, Executable func) {
    state = SET_UP;
    runSetUpsRecursively(test->getParentGroup(), test);
    state = TEST;
    runTest(test, func);
    state = TEAR_DOWN;
    runTearDownsRecursively(test->getParentGroup(), test);
    state = INACTIVE;
    test->setExecuted();
}

void Executor::runSetUpsRecursively(Group* group, Test* test) {
    if (group == nullptr) {
        return;
    }
    runSetUpsRecursively(group->getParentGroup(), test);

    bool failed;
    string failMessage;
    try {
        group->setUp();
        failed = false;
    } catch(const exception& e) {
        failed = true;
        failMessage = "An exception was thrown during the "
                      "setUp of group '" + group->getFullDescription()
                      + "': " + e.what();
    } catch(...) {
        failed = true;
        failMessage = "A non-exception object was thrown during the "
                      "setUp of group '" + group->getFullDescription() + "'.";
    }
    if (failed) {
        test->setFailure(failMessage);
    }
}

void Executor::runTest(Test *test, Executable func) {
    try {
        func();
    } catch(const ExpectationFailed& failure) {
        test->setFailure(failure);
    } catch(const exception& e) {
        test->setFailure(
                "An exception was thrown during test: " + string(e.what())
        );
    } catch(...) {
        test->setFailure("A non-exception object was thrown during test");
    }
}

void Executor::runTearDownsRecursively(Group* group, Test* test) {
    if (group == nullptr) {
        return;
    }

    bool failed;
    string failMessage;
    try {
        group->tearDown();
        failed = false;
    } catch(const exception& e) {
        failed = true;
        failMessage = "An exception was thrown during the "
                      "tearDown of group '" + group->getFullDescription()
                      + "': " + e.what();
    } catch(...) {
        failed = true;
        failMessage = "A non-exception object was thrown during the "
                      "tearDown of group '" + group->getFullDescription()
                      + "'.";
    }
    if (failed) {
        test->setFailure(failMessage);
    }

    runTearDownsRecursively(group->getParentGroup(), test);
}

}
