#pragma once

#include "common/interproc/worker_subprocess.hpp"
#include "kktest/core/executor.hpp"

namespace kktest {

class RunningTest {
 public:
    explicit RunningTest(Test test);

    void startExecution(Executor* executor);

    bool finishedCurrentExecution();

    bool finishedAllExecutions() const;

    void executeBoxed(Executor* executor, interproc::PipeWriter* pipe) const;

    ExecutedTest toExecutedTest() &&;

 private:
    Test test;
    std::vector<ExecutedTest::Info> executions;
    std::unique_ptr<interproc::WorkerSubprocess> currentExecution = nullptr;
};

class BoxExecutor: public Executor {
 public:
    BoxExecutor(OnTestFinished onTestFinished, std::size_t numBoxes);

    ~BoxExecutor() override = default;

    void execute(Test test) override;

    void finalize() override;

 private:
    void ensureEmptyBoxes(std::size_t numContainers);

    std::size_t numBoxes;
    std::vector<RunningTest> activeBoxes;
};

}
