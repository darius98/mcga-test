#ifndef KKTEST_TESTING_BOX_EXECUTOR_H_
#define KKTEST_TESTING_BOX_EXECUTOR_H_

#include "executor.hpp"


namespace kktest {

class BoxExecutor: public Executor {
public:
    explicit BoxExecutor(std::string _binaryPath);

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func) override;
private:
    std::string binaryPath;
    bool copiedBinary = false;
};

}

#endif
