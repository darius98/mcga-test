#ifndef KKTEST_TESTING_EXECUTOR_H_
#define KKTEST_TESTING_EXECUTOR_H_

#include "executable.hpp"
#include "group.hpp"
#include "test.hpp"


namespace kktest {

class Executor {
public:
    virtual bool isDuringTest() const = 0;

    virtual void checkIsInactive(const std::string& methodName) const = 0;

    virtual void execute(const std::vector<Group*>& groups,
                         Test* test,
                         Executable func) = 0;
protected:
    int currentTestIndex = 0;
};

}

#endif
