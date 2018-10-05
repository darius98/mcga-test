#ifndef KKTEST_TESTING_DRIVER_H_
#define KKTEST_TESTING_DRIVER_H_

#include <stack>

#include "testing/group.hpp"


namespace kktest {

class TestingDriver {
public:
    ~TestingDriver();

    void addGroup(Group* currentGroup, const std::function<void()>& func);

    void addTest(Test* currentTest, const std::function<void()>& func);

    void addSetUp(const std::function<void()>& func);

    void addTearDown(const std::function<void()>& func);

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

    void validate(const std::string& methodName);

    void validateStartSetUp();

    void validateStartTearDown();

    std::string getTestFullName(Test* currentTest) const;

    void executeSetUps(Test* currentTest);

    void executeTest(Test* currentTest, const std::function<void()>& testFunc);

    void executeTearDowns(Test* currentTest);

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
