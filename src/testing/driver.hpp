#ifndef KKTEST_TESTING_DRIVER_H_
#define KKTEST_TESTING_DRIVER_H_

#include <iostream>
#include <stack>

#include "group.hpp"
#include "executor.hpp"


namespace kktest {

class TestingDriver {
public:
    static void initGlobal(const std::string& executorName);

    static int destroyGlobal();

    static void generateTestReport(std::ostream& report);

    ~TestingDriver();

    void addGroup(Group* currentGroup, Executable func);

    void addTest(Test* currentTest, Executable func);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

    int getNumFailedTests();

private:
    explicit TestingDriver(const std::string& executorName);

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
    Executor* executor;
    bool shouldLog;

    static bool isDuringTest();

    static TestingDriver* getGlobalDriver();

    static TestingDriver* globalTestingDriver;

    friend class Matcher;
    friend class Definer;
};

}

#endif
