#ifndef RUNTIME_TESTING_TESTING_DRIVER_H_
#define RUNTIME_TESTING_TESTING_DRIVER_H_

#include <ostream>
#include <stack>

#include "group.hpp"

namespace runtime_testing {

class TestingDriver {
public:
    explicit TestingDriver(std::ostream& log);

    bool isDuringTest();

    void validateStartGroup();

    void validateStartTest();

    void validateStartSetUp(const char* fileName=nullptr,
                            const int& lineNumber=0);

    void validateStartTearDown(const char* fileName=nullptr,
                               const int& lineNumber=0);

    void addGroup(Group* currentGroup, const std::function<void()>& func);

    void addTest(Test* currentTest, const std::function<void()>& func);

    void addSetUp(const std::function<void()>& func);

    void addTearDown(const std::function<void()>& func);

    void generateTestReport(std::ostream& report, const char* fileName);

    int getNumFailedTests();

private:
    enum DriverState {
        TOP_LEVEL,
        GROUP,
        TEST,
        SET_UP,
        TEAR_DOWN,
    };

    void validate(const std::string& methodName,
                  const char* fileName=nullptr,
                  const int& lineNumber=0);

    std::vector<Group*> groupStack;
    std::stack<DriverState> state;
    std::ostream& log;
};

}

#endif
