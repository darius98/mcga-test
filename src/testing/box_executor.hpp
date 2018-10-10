#ifndef KKTEST_TESTING_BOX_EXECUTOR_H_
#define KKTEST_TESTING_BOX_EXECUTOR_H_

#include "executor.hpp"


namespace kktest {

class BoxExecutor: public Executor {
public:
    explicit BoxExecutor(int testIndexToRun, std::string _binaryPath);

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

private:
    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func,
                 int testIndex) override;

    std::string binaryPath;
    bool copiedBinary;
};

}

#endif
