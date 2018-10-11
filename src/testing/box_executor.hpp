#ifndef KKTEST_TESTING_BOX_EXECUTOR_H_
#define KKTEST_TESTING_BOX_EXECUTOR_H_

#include <vector>

#include "box_wrapper.hpp"
#include "executor.hpp"


namespace kktest {

class BoxExecutor: public Executor {
public:
    BoxExecutor(int testIndexToRun, bool quiet, std::string binaryPath);

    ~BoxExecutor();

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

    void finalize() override;

private:
    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func) override;

    int pollForEmptyBox();

    bool tryFinalizeBox(int boxId);

    std::vector<std::pair<BoxWrapper*, Test*>> boxes;
    int currentBoxIndex = 0;
};

}

#endif
