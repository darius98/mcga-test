#ifndef KKTEST_PLUGINS_BOX_PLUGIN_BOX_EXECUTOR_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_BOX_EXECUTOR_H_

#include <vector>

#include <core/executor.hpp>
#include "test_container.hpp"


namespace kktest {

class BoxExecutor: public Executor {
public:
    BoxExecutor(int numBoxes);

    void finalize() override;

private:
    void execute(Test* test, Executable func, Executable after) override;

    TestContainer& findEmptyContainer();

    bool tryFinalizeContainer(TestContainer& container);

    std::vector<TestContainer> containers;
};

}

#endif
