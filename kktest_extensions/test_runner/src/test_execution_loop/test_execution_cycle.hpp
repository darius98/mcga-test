#ifndef KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_HPP_
#define KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_HPP_

#include <map>
#include <queue>
#include <string>

#include <messaging.hpp>

#include <test_execution_loop/kktest_lib_info.hpp>

namespace kktest {
namespace test_runner {

class TestExecutionCycle {
 public:
    explicit TestExecutionCycle(const std::string& _testPath,
                                int _maxParallelTests,
                                const std::function<void(const KKTestCaseInfo&)>& _onInfoCallback);

    bool isStarted() const;

    void start();

    void step();

    bool isDone() const;

 private:
    void processMessages(bool block);

    void processMessage(const messaging::Message& message);

    bool started;
    std::string testPath;
    int maxParallelTests;
    std::function<void(const KKTestCaseInfo&)> onInfoCallback;

    messaging::InputPipe* pipeWithTestProcess;
    std::string pipeName;
    int pipeFD;
    pid_t testProcessPID;

    KKTestCaseInfo info;
};

}  // namespace test_runner
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_HPP_
