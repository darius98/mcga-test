#ifndef KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_HPP_
#define KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_HPP_

#include <map>
#include <queue>
#include <string>

#include <kktest_common/interproc.hpp>
#include <test_execution_loop/kktest_lib_info.hpp>

namespace kktest {
namespace test_runner {

class TestExecutionCycle {
 public:
    explicit TestExecutionCycle(const String& _testPath,
                                int _maxParallelTests,
                                const std::function<void(const KKTestCaseInfo&)>& _onInfoCallback);

    bool isStarted() const;

    void start();

    void step();

    bool isDone() const;

 private:
    void processMessages(bool block);

    void processMessage(const interproc::Message& message);

    bool started;
    String testPath;
    int maxParallelTests;
    std::function<void(const KKTestCaseInfo&)> onInfoCallback;

    interproc::PipeReader* pipeWithTestProcess;
    String pipeName;
    interproc::SubprocessHandler* testProcess;

    KKTestCaseInfo info;
};

}  // namespace test_runner
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_TEST_RUNNER_SRC_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_HPP_
