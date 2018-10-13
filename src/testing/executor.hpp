#ifndef KKTEST_TESTING_EXECUTOR_H_
#define KKTEST_TESTING_EXECUTOR_H_

#include <set>

#include "executable.hpp"
#include "group.hpp"
#include "test.hpp"


namespace kktest {

class Executor {
public:
    typedef std::function<void(Test*)> Hook;

    explicit Executor(int _testIndexToRun);

    virtual bool isDuringTest() const = 0;

    virtual void checkIsInactive(const std::string& methodName) const = 0;

    virtual void finalize() = 0;

    void executeTest(const std::vector<Group*>& groups,
                     Test* test,
                     Executable func);

    void addAfterTestHook(Hook hook);

protected:
    void afterTest(Test* test);

private:
    virtual void execute(const std::vector<Group*>& groups,
                         Test* test,
                         Executable func) = 0;

    int testIndexToRun;
    std::vector<Hook> afterTestHooks;
};

}

#endif
