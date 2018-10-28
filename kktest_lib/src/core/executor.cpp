#include <iostream>

#include "executor.hpp"

using namespace std;

namespace kktest {

Executor::Executor(int _testIndexToRun): testIndexToRun(_testIndexToRun) {}

void Executor::executeTest(Test* test, Executable func) {
    if (testIndexToRun == 0 || testIndexToRun == test->getIndex()) {
        beforeTest(test);
        execute(test, func);
    }
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

}
