#ifndef KKTEST_CORE_BOX_EXECUTOR_HPP_
#define KKTEST_CORE_BOX_EXECUTOR_HPP_

#include <set>

#include "executor.hpp"
#include "test_container.hpp"

namespace kktest {

class BoxExecutor: public Executor {
 public:
    BoxExecutor(const OnTestFinishedCallback& onTestFinishedCallback,
                std::size_t _maxNumContainers);

    void finalize() override;

 private:
    void execute(Test* test, Executable func) override;

    void ensureFreeContainers(std::size_t numContainers);

    std::size_t maxNumContainers;
    std::set<TestContainer*> openContainers;
};

}

#endif
