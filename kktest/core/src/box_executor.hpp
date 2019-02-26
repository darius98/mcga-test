#ifndef KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_

#include "common/interproc/src/worker_subprocess.hpp"
#include "kktest/core/src/executor.hpp"

namespace kktest {

// TODO(darius98): Make this a class!
struct RunningTest {
    // TODO(darius98): Make these private!
    Test test;
    std::vector<ExecutedTest::Info> executions;
    std::unique_ptr<interproc::WorkerSubprocess> process = nullptr;

    explicit RunningTest(Test test);

    void startExecution(Executor* executor);

    bool finishedCurrentExecution();

    bool finishedAllExecutions() const;

    void executeBoxed(Executor* executor, interproc::PipeWriter* pipe) const;

    ExecutedTest toExecutedTest() &&;
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

#endif
