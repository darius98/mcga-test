#pragma once

#include <mcga/proc.hpp>

#include "disallow_copy_and_move.hpp"
#include "executor.hpp"

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
  private:
    static constexpr auto loopSleepTime = std::chrono::milliseconds(5);

  public:
    explicit BoxExecutor(std::size_t numBoxes);

    MCGA_DISALLOW_COPY_AND_MOVE(BoxExecutor);

    ~BoxExecutor() override = default;

    void execute(Test test) override;

    void finalize() override;

    void emitWarning(const std::string& message) override;

    Type getType() const override;

    void setCurrentTestingSubprocessPipe(mcga::proc::PipeWriter* pipe);

  private:
    void ensureEmptyBoxes(std::size_t numContainers);

    mcga::proc::PipeWriter* currentTestingSubprocessPipe = nullptr;
    std::size_t numBoxes;
    std::vector<RunningTest> activeBoxes;

    friend class RunningTest;
};

}  // namespace mcga::test
