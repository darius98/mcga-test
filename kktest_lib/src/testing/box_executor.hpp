#ifndef KKTEST_TESTING_BOX_EXECUTOR_H_
#define KKTEST_TESTING_BOX_EXECUTOR_H_

#include <vector>

#include "box.hpp"
#include "executor.hpp"


namespace kktest {

class TestContainer {
public:
    TestContainer(const std::string& boxId, const std::string& binaryPath);

    void runTest(Test* _test);

    bool tryFinalize(std::function<void(Test*, autojson::JSON)> callback);

private:
    Box box;
    Test* test;
};

class BoxExecutor: public Executor {
public:
    BoxExecutor(int testIndexToRun,
                const std::string& binaryPath,
                int boxIdBegin,
                int boxIdEnd);

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

    void finalize() override;

private:
    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func) override;

    TestContainer& findEmptyContainer();

    bool tryFinalizeContainer(TestContainer& container);

    std::vector<TestContainer> containers;
};

}

#endif
