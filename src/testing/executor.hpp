#ifndef KKTEST_TESTING_EXECUTOR_H_
#define KKTEST_TESTING_EXECUTOR_H_

#include "executable.hpp"
#include "group.hpp"
#include "test.hpp"


namespace kktest {

class Executor {
public:
    explicit Executor(int _testIndexToRun, bool _verbose);

    virtual bool isDuringTest() const = 0;

    virtual void checkIsInactive(const std::string& methodName) const = 0;

    virtual void finalize() = 0;

    void executeTest(const std::vector<Group*>& groups,
                     Test* test,
                     Executable func);

    bool isSingleTestExecutor() const;

    void logTest(Test* test);
private:
    virtual void execute(const std::vector<Group*>& groups,
                         Test* test,
                         Executable func) = 0;

    int testIndexToRun;
    bool verbose;
};

}

#endif
