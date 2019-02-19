#ifndef KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_

#include <vector>

#include "common/interproc/src/worker_subprocess.hpp"
#include "core/src/executor.hpp"

namespace kktest {

struct BoxedTest {
    Test test;
    std::unique_ptr<interproc::WorkerSubprocess> process;

    BoxedTest(Test&& _test, interproc::WorkerSubprocess* _process);
    BoxedTest(BoxedTest&& other) noexcept;

    BoxedTest& operator=(BoxedTest&& other) noexcept;
};

class BoxExecutor: public Executor {
 public:
    BoxExecutor(const OnTestFinished& onTestFinished,
                std::size_t _maxNumContainers);

    void finalize() override;

 private:
    void execute(Test&& test, Executable func) override;

    void runContained(GroupPtr group,
                      Executable func,
                      interproc::PipeWriter* pipe);

    void ensureFreeContainers(std::size_t numContainers);

    bool tryCloseContainer(std::vector<BoxedTest>::iterator boxedTest);

    std::size_t maxNumContainers;
    std::vector<BoxedTest> openContainers;
};

}

#endif
