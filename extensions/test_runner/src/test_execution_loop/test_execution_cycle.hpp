#ifndef KKTEST_EXTENSIONS_TEST_RUNNER_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_HPP_
#define KKTEST_EXTENSIONS_TEST_RUNNER_TEST_EXECUTION_LOOP_TEST_EXECUTION_CYCLE_HPP_

#include <map>
#include <queue>

#include <kktest_common/interproc.hpp>
#include <test_execution_loop/kktest_lib_info.hpp>

namespace kktest {
namespace test_runner {

class TestExecutionCycle {
 public:
    typedef std::function<void(const KKTestCaseInfo&)> OnInfoCallback;

    explicit TestExecutionCycle(
        const String& _testPath,
        int _maxParallelTests,
        const OnInfoCallback& _onInfoCallback);

    bool isStarted() const;

    void start();

    void step();

    bool isDone() const;

 private:
    void processMessages(bool block);

    void processMessage(const interproc::Message& message);

    bool started = false;

    String testPath;
    int maxParallelTests;
    OnInfoCallback onInfoCallback;

    String pipeName;

    interproc::PipeReader* pipeWithTestProcess = nullptr;
    interproc::SubprocessHandler* testProcess = nullptr;

    KKTestCaseInfo info;
};

}
}

#endif
