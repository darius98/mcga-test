#include <iostream>

#include <EasyFlags.hpp>

#include "matcher/matcher.hpp"
#include "testing/driver.hpp"

using namespace easyflags;
using namespace std;

AddArgument(int, flagEnableLogging)
    .Name("enable-logging")
    .ArgumentType("bool")
    .Description("Enable STDOUT logging for this test run")
    .DefaultValue(1).ImplicitValue(1);


namespace kktest {

TestingDriver* TestingDriver::globalTestingDriver = nullptr;

TestingDriver* TestingDriver::getGlobalDriver() {
    if (globalTestingDriver == nullptr) {
        std::cout << "Global testing driver not defined. Cannot continue.\n";
        exit(1);
    }
    return globalTestingDriver;
}

void TestingDriver::initGlobal() {
    if (globalTestingDriver != nullptr) {
        throw runtime_error("Testing driver cannot be initialized: "
                            "it already exists.");
    }
    globalTestingDriver = new TestingDriver();
}

void TestingDriver::destroyGlobal() {
    delete globalTestingDriver;
    globalTestingDriver = nullptr;
}

TestingDriver::TestingDriver():
        shouldLog(flagEnableLogging != 0), executor(new Executor()) {
    groupStack = {new Group()};
    state.push(DriverState::TOP_LEVEL);
}

TestingDriver::~TestingDriver() {
    delete groupStack[0];
    delete executor;
}

bool TestingDriver::isDuringTest() {
    return globalTestingDriver != nullptr &&
            globalTestingDriver->state.top() == DriverState::TEST;
}

void TestingDriver::addGroup(Group* currentGroup, Executable func) {
    checkIsNotAlreadyExecuting("group");
    groupStack.back()->subGroups.push_back(currentGroup);
    groupStack.push_back(currentGroup);
    state.push(DriverState::GROUP);
    executor->executeGroup(currentGroup, func);
    state.pop();
    groupStack.pop_back();
}

void TestingDriver::addTest(Test* currentTest, Executable func) {
    checkIsNotAlreadyExecuting("test");
    groupStack.back()->tests.push_back(currentTest);
    log(getTestFullName(currentTest), ": ");
    execute(currentTest, func);
    if (currentTest->failure) {
        log("FAILED\n\t", currentTest->failure->getMessage(), "\n");
    } else {
        log("PASSED\n");
    }
    for (Group* g: groupStack) {
        g->numTests += 1;
        g->numFailedTests += (currentTest->failure != nullptr);
    }
    Matcher::cleanupMatchersCreatedDuringTests();
}

void TestingDriver::execute(Test* currentTest, Executable func) {
    state.push(DriverState::SET_UP);
    executor->executeSetUps(groupStack, currentTest);
    state.pop();
    state.push(DriverState::TEST);
    executor->executeTest(currentTest, func);
    state.pop();
    state.push(DriverState::TEAR_DOWN);
    executor->executeTearDowns(groupStack, currentTest);
    state.pop();
}

void TestingDriver::addSetUp(Executable func) {
    checkIsNotAlreadyExecuting("setUp");
    groupStack.back()->setSetUp(func);
}

void TestingDriver::addTearDown(Executable func) {
    checkIsNotAlreadyExecuting("tearDown");
    groupStack.back()->setTearDown(func);
}

void TestingDriver::generateTestReport(ostream& report) {
    report << groupStack[0]->generateReport().stringify(false);
}

int TestingDriver::getNumFailedTests() {
    return groupStack[0]->numFailedTests;
}

void TestingDriver::checkIsNotAlreadyExecuting(const string& methodName) {
    if (state.top() == DriverState::TEST) {
        throw runtime_error("Cannot call " + methodName + " within test!");
    }
    if (state.top() == DriverState::SET_UP) {
        throw runtime_error("Cannot call " + methodName + " within setUp!");
    }
    if (state.top() == DriverState::TEAR_DOWN) {
        throw runtime_error("Cannot call " + methodName + " within tearDown!");
    }
}

string TestingDriver::getTestFullName(Test* currentTest) const {
    string file;
    string groupStackFullName;

    if (!currentTest->file.empty()) {
        file = currentTest->file + ":" + to_string(currentTest->line) + ": ";
    }
    for (Group* g: groupStack) {
        if (g != groupStack[0]) {
            if (!groupStackFullName.empty()) {
                groupStackFullName += "::";
            }
            groupStackFullName += g->description;
        }
    }
    if (!groupStackFullName.empty()) {
        groupStackFullName += "::";
    }
    return file + groupStackFullName + currentTest->description;
}

}
