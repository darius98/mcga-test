#ifndef KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_

#include <set>

#include "common/interproc/src/worker_subprocess.hpp"
#include "core/src/executor.hpp"

namespace kktest {

struct BoxedTest {
    Test* test;
    interproc::WorkerSubprocess* process;

    BoxedTest(Test* _test, interproc::WorkerSubprocess* _process);
    BoxedTest(BoxedTest&& other) noexcept;
    BoxedTest(const BoxedTest& other) = delete;

    ~BoxedTest();

    bool operator<(const BoxedTest& other) const;
};

class BoxExecutor: public Executor {
 public:
    BoxExecutor(const OnTestFinishedCallback& onTestFinishedCallback,
                std::size_t _maxNumContainers);

    void finalize() override;

 private:
    void execute(Test* test, Executable func) override;

    void runContained(Test* test, Executable func, interproc::PipeWriter* pipe);

    void ensureFreeContainers(std::size_t numContainers);

    std::size_t maxNumContainers;
    std::set<BoxedTest> openContainers;
};

}

#endif
