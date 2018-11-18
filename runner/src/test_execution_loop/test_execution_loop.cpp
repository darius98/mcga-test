#include "test_execution_loop.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;

namespace runner {

TestExecutionLoop::TestExecutionLoop(int _concurrentRunningCyclesLimit):
        concurrentRunningCyclesLimit(_concurrentRunningCyclesLimit),
        testLogger(cout, false) {}

void TestExecutionLoop::addToLoop(const std::string& testPath, int maxParallelTests) {
    cycles.emplace_back(
        testPath,
        maxParallelTests,
        [this](const KKTestCaseInfo& info) {
            if (info.finished && info.finishedWithError) {
                testLogger.logFatalError(info.errorMessage, info.testExecutablePath);
            } else {
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
        }
    );
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
        this_thread::sleep_for(milliseconds(10));
    }
    testLogger.logFinalInformation(true);
    return failedAnyTest;
}

}
