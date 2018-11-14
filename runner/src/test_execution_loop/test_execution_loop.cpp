#include "test_execution_loop.hpp"

#include <iostream>

using namespace std;

namespace kktest_runner {

TestExecutionLoop::TestExecutionLoop(int _concurrentRunningCyclesLimit):
        concurrentRunningCyclesLimit(_concurrentRunningCyclesLimit) {}

void TestExecutionLoop::addToLoop(const std::string& testPath, int maxParallelTests) {
    cycles.push_back(TestExecutionCycle(
            testPath,
            maxParallelTests,
            [this](const KKTestCaseInfo& info) {
                cout << "Finished test " << info.tests.back().index << " from suite " << info.testExecutablePath << "\n";
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

void TestExecutionLoop::join() {
    while (!isEmpty()) {
        step();
    }
}

}
