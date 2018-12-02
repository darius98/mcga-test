#ifndef KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_LOOP_HPP_
#define KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_LOOP_HPP_

#include <string>
#include <vector>

#include <kktest_ext/feedback.hpp>

#include <test_execution_loop/test_execution_cycle.hpp>

namespace kktest {
namespace test_runner {

class TestExecutionLoop {
 public:
    explicit TestExecutionLoop(int _concurrentRunningCyclesLimit);

    void addToLoop(const kktest::String& testPath, int maxParallelTests);

    bool isEmpty() const;

    void step();

    int join();

 private:
    bool failedAnyTest = false;
    kktest::feedback::TestLogger testLogger;
    int concurrentRunningCyclesLimit;
    int runningCycles = 0;
    std::vector<TestExecutionCycle> cycles;
};

}  // namespace test_runner
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_LOOP_HPP_
