#ifndef KKTEST_TESTING_DRIVER_H_
#define KKTEST_TESTING_DRIVER_H_

#include <iostream>
#include <stack>

#include "testing/group.hpp"
#include "testing/executor.hpp"


namespace kktest {

class TestingDriver {
public:
    ~TestingDriver();

    void addGroup(Group* currentGroup, Executable func);

    void addTest(Test* currentTest, Executable func);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

    void generateTestReport(std::ostream& report);

    int getNumFailedTests();
private:
    enum DriverState {
        TOP_LEVEL,
        GROUP,
        TEST,
        SET_UP,
        TEAR_DOWN,
    };

    TestingDriver();

    void execute(Test* currentTest, Executable func);

    void checkIsNotAlreadyExecuting(const std::string &methodName);

    void checkCurrentGroupHasNoSetUp();

    void checkCurrentGroupHasNoTearDown();

    std::string getTestFullName(Test* currentTest) const;

    template<class T>
    void log(const T& object) {
        if (shouldLog) {
            std::cout << object;
        }
    }

    template<class T, class... Args>
    void log(const T& object, const Args... args) {
        log(object);
        log(args...);
    };

    std::vector<Group*> groupStack;
    std::stack<DriverState> state;
    Executor* executor;
    bool shouldLog;

    static TestingDriver* getGlobalDriver();

    static void initGlobal();

    static void destroyGlobal();

    static bool isDuringTest();

    static TestingDriver* globalTestingDriver;

    friend class Matcher;
    friend class Definer;
    friend void initializeTestingDriver(int argc, char** argv);
    friend int finalizeTesting();
};

}

#endif
