#ifndef KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_

#include <set>

#include "common/interproc/src/timed_worker_subprocess.hpp"
#include "core/src/executor.hpp"

namespace kktest {

class BoxExecutor: public Executor {
 public:
    BoxExecutor(const OnTestFinishedCallback& onTestFinishedCallback,
                std::size_t _maxNumContainers);

    void finalize() override;

 private:
    void execute(Test* test, Executable func) override;

    void runContained(Test* test, Executable func, interproc::PipeWriter* pipe);

    void onContainerMessage(Test* test, interproc::Message& message);

    void ensureFreeContainers(std::size_t numContainers);

    std::size_t maxNumContainers;
    std::set<interproc::TimedWorkerSubprocess*> openContainers;
};

}

#endif
