#include <iostream>

#include <EasyFlags.hpp>

#include "executor.hpp"

using namespace std;

AddArgument(int, argumentTestIndex)
    .ArgumentType("int ")
    .Name("test")
    .Short("t")
    .Description("Index of the test to run (defaults to 0 - run all tests).")
    .DefaultValue(0);

namespace kktest {

Executor::Executor() {
    if (argumentTestIndex != 0) {
        addAfterTestHook([this](Test *test) {
            cout << test->toJSON();
            cout.flush();
        });
    }
}

Executor::~Executor() {
}

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
    if (argumentTestIndex == 0 || argumentTestIndex == test->getIndex()) {
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
