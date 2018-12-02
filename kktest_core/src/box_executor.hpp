#ifndef KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_

#include <set>

#include "executor.hpp"
#include "test_container.hpp"

namespace kktest {

class BoxExecutor: public Executor {
 public:
    explicit BoxExecutor(std::size_t _maxNumContainers);

    void finalize() override;

 private:
    void execute(Test* test, Executable func) override;

    void ensureFreeContainers(std::size_t numContainers);

    std::size_t maxNumContainers;
    std::set<TestContainer*> openContainers;
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_BOX_EXECUTOR_HPP_
