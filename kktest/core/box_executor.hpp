#pragma once

#include <mcga/proc.hpp>

#include "kktest/core/executor.hpp"

namespace mcga::test {

class BoxExecutor;

class RunningTest {
  public:
    explicit RunningTest(Test test, BoxExecutor* executor);

    void startExecution();

    bool finishedCurrentExecution();

    void executeBoxed(mcga::proc::PipeWriter* pipe) const;

    bool finishedAllExecutions() const;

    ExecutedTest toExecutedTest() &&;

  private:
    Test test;
    std::vector<ExecutedTest::Info> executions;
    std::unique_ptr<mcga::proc::WorkerSubprocess> currentExecution = nullptr;

    BoxExecutor* executor;
};

class BoxExecutor : public Executor {
  public:
    explicit BoxExecutor(std::size_t numBoxes);

    ~BoxExecutor() override = default;

    void execute(Test test) override;

    void finalize() override;

    void emitWarning(const std::string& message) override;

    Type getType() const override;

    void setCurrentTestingSubprocessPipe(mcga::proc::PipeWriter* pipe);

  private:
    void ensureEmptyBoxes(std::size_t numContainers);

    mcga::proc::PipeWriter* currentTestingSubprocessPipe;
    std::size_t numBoxes;
    std::vector<RunningTest> activeBoxes;

    friend class RunningTest;
};

}  // namespace mcga::test
