#ifndef KKTEST_PLUGINS_BOX_PLUGIN_BOX_EXECUTOR_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_BOX_EXECUTOR_H_

#include <set>

#include <core/executor.hpp>
#include <plugins/box_plugin/test_container.hpp>

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

}

#endif
