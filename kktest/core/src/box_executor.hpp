#ifndef KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_

#include "common/interproc/src/worker_subprocess.hpp"
#include "kktest/core/src/executor.hpp"

namespace kktest {

struct BoxedTest {
    Test test;
    std::function<void()> testFunc;
    std::vector<ExecutedTest::Info> executions;
    std::unique_ptr<interproc::WorkerSubprocess> process;
};

class BoxExecutor: public Executor {
 public:
    BoxExecutor(OnTestFinished onTestFinished, std::size_t numBoxes);

    ~BoxExecutor() override = default;

    void finalize() override;

 private:
    void execute(Test&& test, Executable func) override;

    void runBoxed(GroupPtr group, Executable func, interproc::PipeWriter* pipe);

    void ensureEmptyBoxes(std::size_t numContainers);

    bool tryCloseBox(std::vector<BoxedTest>::iterator boxedTest);

    std::size_t numBoxes;
    std::vector<BoxedTest> activeBoxes;
};

}

#endif
