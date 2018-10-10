#include <iostream>

#include "executor.hpp"

using namespace std;

namespace kktest {

Executor::Executor(int _testIndexToRun, bool _verbose):
    testIndexToRun(_testIndexToRun), currentTestIndex(0), verbose(_verbose) {}

void Executor::executeTest(const vector<Group*>& groups,
                           Test* test,
                           Executable func) {
    currentTestIndex += 1;
    if (testIndexToRun == 0 || testIndexToRun == currentTestIndex) {
        execute(groups, test, func, currentTestIndex);
    }
}

bool Executor::isSingleTestExecutor() const {
    return testIndexToRun != 0;
}

void Executor::logTest(Test* test) {
    if (!verbose) {
        return;
    }
    cout << test->getFullDescription()
         << ": "
         << (test->isFailed() ? "FAILED" : "PASSED")
         << "\n";
    if (test->isFailed()) {
        cout << "\t" << test->getFailureMessage() << "\n";
    }
}

}
