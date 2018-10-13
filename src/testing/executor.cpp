#include <iostream>

#include "executor.hpp"

using namespace std;

namespace kktest {

Executor::Executor(int _testIndexToRun): testIndexToRun(_testIndexToRun) {}

void Executor::executeTest(const vector<Group*>& groups,
                           Test* test,
                           Executable func) {
    if (testIndexToRun == 0 || testIndexToRun == test->getIndex()) {
        execute(groups, test, func);
    }
}

void Executor::addAfterTestHook(Hook hook) {
    afterTestHooks.push_back(hook);
}

void Executor::afterTest(Test* test) {
    for (Hook hook: afterTestHooks) {
        hook(test);
    }
}

}
