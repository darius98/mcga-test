#ifndef KKTEST_PLUGINS_BOX_PLUGIN_BOX_EXECUTOR_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_BOX_EXECUTOR_H_

#include <vector>

#include <core/executor.hpp>
#include "box.hpp"


namespace kktest {

class TestContainer {
public:
    TestContainer(const std::string& binaryPath);

    void runTest(Test* _test);

    bool tryFinalize(std::function<void(Test*, const std::string&)> callback);

private:
    Box box;
    Test* test;
};

class BoxExecutor: public Executor {
public:
    BoxExecutor(const std::string& binaryPath, int numBoxes);

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

    void finalize() override;

private:
    void execute(Test* test, Executable func) override;

    TestContainer& findEmptyContainer();

    bool tryFinalizeContainer(TestContainer& container);

    std::vector<TestContainer> containers;
};

}

#endif
