#ifndef KKTEST_TESTING_DRIVER_H_
#define KKTEST_TESTING_DRIVER_H_

#include <ostream>
#include <stack>

#include "testing/group.hpp"


namespace kktest {

class TestingDriver {
public:
    static TestingDriver* getOrCreateGlobalDriver();

    static void init(std::ostream& logger);

    static void destroy();

    static bool isDuringTest();

private:
    static TestingDriver* globalTestingDriver;

public:
    explicit TestingDriver(std::ostream* logger=&std::cout);

    ~TestingDriver();

    void addGroup(Group* currentGroup, const std::function<void()>& func);

    void addTest(Test* currentTest, const std::function<void()>& func);

    void addSetUp(const std::function<void()>& func);

    void addTearDown(const std::function<void()>& func);

    int generateTestReport(std::ostream& report);

    int getNumFailedTests();

private:
    enum DriverState {
        TOP_LEVEL,
        GROUP,
        TEST,
        SET_UP,
        TEAR_DOWN,
    };

    void validate(const std::string& methodName);

    void validateStartGroup();

    void validateStartTest();

    void validateStartSetUp();

    void validateStartTearDown();

    std::string getTestFullName(Test* currentTest) const;

    void executeSetUps(Test* currentTest);

    void executeTest(Test* currentTest, const std::function<void()>& testFunc);

    void executeTearDowns(Test* currentTest);

    template<class T>
    void log(const T& object) {
        if (logger != nullptr) {
            (*logger) << object;
        }
    }

    template<class T, class... Args>
    void log(const T& object, const Args... args) {
        log(object);
        log(args...);
    };

    std::vector<Group*> groupStack;
    std::stack<DriverState> state;
    std::ostream* logger;
};

}

#endif
