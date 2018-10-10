#include "executor.hpp"

using namespace std;

namespace kktest {

Executor::Executor(int _testIndexToRun):
    testIndexToRun(_testIndexToRun), currentTestIndex(0) {}

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

}
