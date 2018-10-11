#include <iostream>

#include "executor.hpp"

using namespace std;

namespace kktest {

bool Executor::AscendingByTestIndex::operator()(Test* const& a,
                                                Test* const& b) {
    return a->getIndex() < b->getIndex();
}

Executor::Executor(int _testIndexToRun, bool _quiet):
    testIndexToRun(_testIndexToRun), quiet(_quiet) {}

void Executor::executeTest(const vector<Group*>& groups,
                           Test* test,
                           Executable func) {
    if (testIndexToRun == 0 || testIndexToRun == test->getIndex()) {
        execute(groups, test, func);
    }
}

void Executor::addAfterTestHook(CopyableExecutable hook) {
    afterTestHooks.push_back(hook);
}

bool Executor::isSingleTestExecutor() const {
    return testIndexToRun != 0;
}

void Executor::enqueueTestForLogging(Test* test) {
    for (Executable hook: afterTestHooks) {
        hook();
    }
    if (quiet) {
        return;
    }
    if (isSingleTestExecutor()) {
        if (test->isFailed()) {
            cout << test->getFailureMessage();
        }
        return;
    }
    loggingQueue.insert(test);
    while (!loggingQueue.empty() &&
                (*loggingQueue.begin())->getIndex() == testsLogged + 1) {
        Test* test = *loggingQueue.begin();
        cout << test->getFullDescription()
             << ": "
             << (test->isFailed() ? "FAILED" : "PASSED")
             << "\n";
        if (test->isFailed()) {
            cout << "\t" << test->getFailureMessage() << "\n";
        }
        testsLogged += 1;
        loggingQueue.erase(loggingQueue.begin());
    }
}

}
