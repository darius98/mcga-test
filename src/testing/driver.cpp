#include <iostream>

#include <EasyFlags.hpp>

#include <matcher/matcher.hpp>

#include "driver.hpp"

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
        cout << "Global testing driver not defined. Cannot continue.\n";
        exit(1);
    }
    return globalTestingDriver;
}

void TestingDriver::initGlobal(const string& executorName) {
    if (globalTestingDriver != nullptr) {
        throw runtime_error("Testing driver cannot be initialized: "
                            "it already exists.");
    }
    globalTestingDriver = new TestingDriver(executorName);
}

void TestingDriver::generateTestReport(ostream& report) {
    report << getGlobalDriver()->groupStack[0]->generateReport().stringify(0);
}

int TestingDriver::destroyGlobal() {
    TestingDriver* driver = getGlobalDriver();
    int status = driver->getNumFailedTests();
    delete driver;
    globalTestingDriver = nullptr;
    return status;
}

TestingDriver::TestingDriver(const string& executorName):
        shouldLog(flagEnableLogging != 0),
        executor(new Executor(executorName)),
        groupStack({new Group()}) {}

TestingDriver::~TestingDriver() {
    delete groupStack[0];
    delete executor;
}

bool TestingDriver::isDuringTest() {
    return globalTestingDriver != nullptr &&
            globalTestingDriver->executor->isDuringTest();
}

void TestingDriver::addGroup(Group* currentGroup, Executable func) {
    executor->checkIsInactive("group");
    groupStack.back()->subGroups.push_back(currentGroup);
    currentGroup->parentGroup = groupStack.back();
    groupStack.push_back(currentGroup);
    executor->executeGroup(currentGroup, func);
    groupStack.pop_back();
}

void TestingDriver::addTest(Test* currentTest, Executable func) {
    executor->checkIsInactive("test");
    groupStack.back()->tests.push_back(currentTest);
    currentTest->parentGroup = groupStack.back();
    log(currentTest->getFullDescription(), ": ");
    executor->execute(groupStack, currentTest, func);
    if (currentTest->isFailed()) {
        log("FAILED\n\t", currentTest->getFailureMessage(), "\n");
    } else {
        log("PASSED\n");
    }
    for (Group* g: groupStack) {
        g->numTests += 1;
        g->numFailedTests += currentTest->isFailed();
    }
    Matcher::cleanupMatchersCreatedDuringTests();
}

void TestingDriver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->setSetUp(func);
}

void TestingDriver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->setTearDown(func);
}

int TestingDriver::getNumFailedTests() {
    return groupStack[0]->numFailedTests;
}

}
