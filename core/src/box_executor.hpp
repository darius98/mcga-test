#ifndef KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_

#include <vector>

#include "common/interproc/src/worker_subprocess.hpp"
#include "core/src/executor.hpp"

namespace kktest {

struct BoxedTest {
    Test test;
    std::unique_ptr<interproc::WorkerSubprocess> process;

    BoxedTest(Test&& test, interproc::WorkerSubprocess* process);
    BoxedTest(BoxedTest&& other) noexcept;

    BoxedTest& operator=(BoxedTest&& other) noexcept;
};

class BoxExecutor: public Executor {
 public:
    BoxExecutor(OnTestFinished onTestFinished, std::size_t numBoxes);

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
