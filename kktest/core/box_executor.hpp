#pragma once

#include "common/interproc/worker_subprocess.hpp"
#include "kktest/core/executor.hpp"

namespace kktest {

class BoxExecutor;

class RunningTest {
 public:
    explicit RunningTest(Test test, BoxExecutor* executor);

    void startExecution();

    bool finishedCurrentExecution();

    void executeBoxed(interproc::PipeWriter* pipe) const;

    bool finishedAllExecutions() const;

    ExecutedTest toExecutedTest() &&;

 private:
    Test test;
    std::vector<ExecutedTest::Info> executions;
    std::unique_ptr<interproc::WorkerSubprocess> currentExecution = nullptr;

    BoxExecutor* executor;
};

class BoxExecutor: public Executor {
 public:
    explicit BoxExecutor(std::size_t numBoxes);

    ~BoxExecutor() override = default;

    void execute(Test test) override;

    void finalize() override;

    void setCurrentTestingSubprocessPipe(interproc::PipeWriter* pipe);

    void handleWarning(const std::string& message) override;

 private:
    void ensureEmptyBoxes(std::size_t numContainers);

    interproc::PipeWriter* currentTestingSubprocessPipe;
    std::size_t numBoxes;
    std::vector<RunningTest> activeBoxes;

 friend class RunningTest;
};

}
