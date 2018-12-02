#ifndef KKTEST_RUNNER_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_H_
#define KKTEST_RUNNER_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_H_

#include <map>
#include <queue>
#include <string>

#include <messaging>

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

}
}

#endif
