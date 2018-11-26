#include <chrono>
#include <iostream>
#include <thread>

#include <test_execution_loop/test_execution_loop.hpp>

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
            switch(info.lastReceived) {
                case KKTestCaseInfo::FINISH_WITH_ERROR:
                    testLogger.logFatalError(info.errorMessage, info.testExecutablePath);
                    break;
                case KKTestCaseInfo::FINISH:
                    break;
                case KKTestCaseInfo::GROUP:
                    testLogger.addGroupInfo(info.groupsReceived.back());
                    break;
                case KKTestCaseInfo::TEST:
                    failedAnyTest |= !info.testsReceived.back().passed;
                    testLogger.logTest(info.testsReceived.back());
                    break;
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
