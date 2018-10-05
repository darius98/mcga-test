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

TestingDriver::TestingDriver(): shouldLog(flagEnableLogging) {
    groupStack = {new Group()};
    state.push(DriverState::TOP_LEVEL);
}

TestingDriver::~TestingDriver() {
    delete groupStack[0];
}

bool TestingDriver::isDuringTest() {
    return globalTestingDriver != nullptr &&
            globalTestingDriver->state.top() == DriverState::TEST;
}

void TestingDriver::validateStartGroup() {
    validate("group");
}

void TestingDriver::validateStartTest() {
    validate("test");
}

void TestingDriver::validateStartSetUp() {
    validate("setUp");
    auto lastGroup = groupStack.back();
    if (lastGroup->hasSetUp) {
        throw runtime_error(
            "Group '" + lastGroup->description + "' already has a setUp!"
        );
    }
}

void TestingDriver::validateStartTearDown() {
    validate("tearDown");
    auto lastGroup = groupStack.back();
    if (lastGroup->hasTearDown) {
        throw runtime_error(
            "Group '" + lastGroup->description + "' already has a tearDown!"
        );
    }
}

void TestingDriver::addGroup(Group* currentGroup,
                             const function<void()> &func) {
    validateStartGroup();
    groupStack.back()->subGroups.push_back(currentGroup);
    groupStack.push_back(currentGroup);
    state.push(DriverState::GROUP);
    bool failed;
    try {
        func();
        failed = false;
    } catch(const exception& e) {
        failed = true;
        log("\nAn exception was thrown inside group '",
            currentGroup->description,
            "': ",
            e.what());
    } catch(...) {
        failed = true;
        log("\nA non-exception object was thrown inside group'",
            currentGroup->description,
            "'");
    }
    if (failed) {
        log(", outside of any test/setUp/tearDown.",
            "\n\n", string(105, '*'), "\n",
            "* Try to place all your testing code inside setUps, "
            "tearDowns or tests to ensure each test is executed. *",
            "\n", string(105, '*'), "\n\n");
    }
    state.pop();
    groupStack.pop_back();
}

void TestingDriver::addTest(Test *currentTest,
                            const function<void()> &func) {
    validateStartTest();
    groupStack.back()->tests.push_back(currentTest);
    log(getTestFullName(currentTest), ": ");
    executeSetUps(currentTest);
    executeTest(currentTest, func);
    executeTearDowns(currentTest);
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

void TestingDriver::addSetUp(const function<void()> &func) {
    validateStartSetUp();
    auto lastGroup = groupStack.back();
    lastGroup->hasSetUp = true;
    lastGroup->setUpFunc = func;
}

void TestingDriver::addTearDown(const function<void()> &func) {
    validateStartTearDown();
    auto lastGroup = groupStack.back();
    lastGroup->hasTearDown = true;
    lastGroup->tearDownFunc = func;
}

void TestingDriver::generateTestReport(ostream& report) {
    report << groupStack[0]->generateReport().stringify(false);
}

int TestingDriver::getNumFailedTests() {
    return groupStack[0]->numFailedTests;
}

void TestingDriver::validate(const string& methodName) {
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

void TestingDriver::executeSetUps(Test* currentTest) {
    state.push(DriverState::SET_UP);
    for (Group* g: groupStack) {
        if (g->hasSetUp) {
            bool failed;
            string failMessage;
            try {
                g->setUpFunc();
                failed = false;
            } catch(const exception& e) {
                failed = true;
                failMessage = "An exception was thrown during the "
                              "setUp of group '" + g->description
                              + "': " + e.what();
            } catch(...) {
                failed = true;
                failMessage = "A non-exception object was thrown during the "
                              "setUp of group '" + g->description + "'.";
            }
            if (failed && currentTest->failure == nullptr) {
                currentTest->failure = new ExpectationFailed(failMessage);
            }
        }
    }
    state.pop();
}

void TestingDriver::executeTest(Test* currentTest,
                                const function<void()>& func) {
    state.push(DriverState::TEST);
    try {
        func();
    } catch(const ExpectationFailed& failure) {
        if (currentTest->failure == nullptr) {
            currentTest->failure = new ExpectationFailed(failure);
        }
    } catch(const exception& e) {
        if (currentTest->failure == nullptr) {
            currentTest->failure = new ExpectationFailed(
                    "An exception was thrown during test: " + string(e.what())
            );
        }
    } catch(...) {
        if (currentTest->failure == nullptr) {
            currentTest->failure = new ExpectationFailed(
                    "A non-exception object was thrown during test"
            );
        }
    }
    state.pop();
}

void TestingDriver::executeTearDowns(Test* currentTest) {
    state.push(DriverState::TEAR_DOWN);
    for (int i = (int)groupStack.size() - 1; i >= 0; -- i) {
        Group* g = groupStack[i];
        if (g->hasTearDown) {
            bool failed;
            string failMessage;
            try {
                g->tearDownFunc();
                failed = false;
            } catch(const exception& e) {
                failed = true;
                failMessage = "An exception was thrown during the "
                              "tearDown of group '" + g->description
                              + "': " + e.what();
            } catch(...) {
                failed = true;
                failMessage = "A non-exception object was thrown during the "
                              "tearDown of group '" + g->description + "'.";
            }
            if (failed && currentTest->failure == nullptr) {
                currentTest->failure = new ExpectationFailed(failMessage);
            }
        }
    }
    state.pop();
}

}
