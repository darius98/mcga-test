#ifndef KKTEST_RUNNER_TEST_EXECUTION_LOOP_TEST_EXECUTION_LOOP_H_
#define KKTEST_RUNNER_TEST_EXECUTION_LOOP_TEST_EXECUTION_LOOP_H_

#include <string>
#include <vector>

#include <logging>
#include "test_execution_cycle.hpp"

namespace kktest_runner {

class TestExecutionLoop {
public:
    explicit TestExecutionLoop(int _concurrentRunningCyclesLimit);

    void addToLoop(const std::string& testPath, int maxParallelTests);

    bool isEmpty() const;

    void step();

    int join();

private:
    bool failedAnyTest = false;
    logging::TestLogger testLogger;
    int concurrentRunningCyclesLimit;
    int runningCycles = 0;
    std::vector<TestExecutionCycle> cycles;
};

}

#endif
