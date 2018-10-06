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

void TestingDriver::initGlobal(const string& executorName) {
    if (globalTestingDriver != nullptr) {
        throw runtime_error("Testing driver cannot be initialized: "
                            "it already exists.");
    }
    globalTestingDriver = new TestingDriver(executorName);
}

void TestingDriver::destroyGlobal() {
    delete globalTestingDriver;
    globalTestingDriver = nullptr;
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
    groupStack.push_back(currentGroup);
    executor->executeGroup(currentGroup, func);
    groupStack.pop_back();
}

void TestingDriver::addTest(Test* currentTest, Executable func) {
    executor->checkIsInactive("test");
    groupStack.back()->tests.push_back(currentTest);
    log(getTestFullName(currentTest), ": ");
    executor->execute(groupStack, currentTest, func);
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

void TestingDriver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->setSetUp(func);
}

void TestingDriver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->setTearDown(func);
}

void TestingDriver::generateTestReport(ostream& report) {
    report << groupStack[0]->generateReport().stringify(false);
}

int TestingDriver::getNumFailedTests() {
    return groupStack[0]->numFailedTests;
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
