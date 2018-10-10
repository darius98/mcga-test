#ifndef KKTEST_TESTING_BOX_EXECUTOR_H_
#define KKTEST_TESTING_BOX_EXECUTOR_H_

#include "box_wrapper.hpp"
#include "executor.hpp"


namespace kktest {

class BoxExecutor: public Executor {
public:
    explicit BoxExecutor(int testIndexToRun, std::string binaryPath);

    ~BoxExecutor();

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

    void finalize() override;

private:
    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func,
                 int testIndex) override;

    BoxWrapper* box;
};

}

#endif
