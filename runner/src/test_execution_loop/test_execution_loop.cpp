#include "test_execution_loop.hpp"

#include <iostream>

using namespace std;

namespace kktest_runner {

TestExecutionLoop::TestExecutionLoop(int _concurrentRunningCyclesLimit):
        concurrentRunningCyclesLimit(_concurrentRunningCyclesLimit),
        testLogger(cout, false) {}

void TestExecutionLoop::addToLoop(const std::string& testPath, int maxParallelTests) {
    cycles.push_back(TestExecutionCycle(
            testPath,
            maxParallelTests,
            [this](const KKTestCaseInfo& info) {
                const TestInfo& testInfo = info.tests.back();
                failedAnyTest |= !testInfo.passed;
                string descriptionPrefix = testInfo.file + ":" + to_string(testInfo.line) + "::";
                descriptionPrefix += info.getRecursiveGroupDescription(testInfo.groupIndex);
                testLogger.logTest(testInfo.index,
                                   descriptionPrefix,
                                   testInfo.description,
                                   testInfo.optional,
                                   testInfo.passed,
                                   testInfo.failureMessage);
            }
    ));
}

bool TestExecutionLoop::isEmpty() const {
    return cycles.empty();
}

void TestExecutionLoop::step() {
    for (auto it = cycles.begin(); it != cycles.end();) {
        if (!it->isStarted()) {
            if (runningCycles < concurrentRunningCyclesLimit) {
                it->start();
                ++ runningCycles;
            }
            ++ it;
        } else {
            it->step();
            if (it->isDone()) {
                it = cycles.erase(it);
                -- runningCycles;
            } else {
                ++ it;
            }
        }
    }
}

int TestExecutionLoop::join() {
    while (!isEmpty()) {
        step();
    }
    testLogger.logFinalInformation();
    return failedAnyTest;
}

}
