#include "driver.hpp"

using namespace matcher;
using namespace std;

namespace runtime_testing {

TestingDriver::TestingDriver(ostream* log): log(log) {
    this->groupStack = {new Group()};
    this->state.push(DriverState::TOP_LEVEL);
}

TestingDriver::~TestingDriver() {
    delete this->groupStack[0];
}

bool TestingDriver::isDuringTest() {
    return this->state.top() == DriverState::TEST;
}

void TestingDriver::validateStartGroup() {
    this->validate("group");
}

void TestingDriver::validateStartTest() {
    this->validate("test");
}

void TestingDriver::validateStartSetUp() {
    this->validate("setUp");
    auto lastGroup = this->groupStack.back();
    if (lastGroup->hasSetUp) {
        throw runtime_error(
            string("Group '") + lastGroup->description
            + "' already has a setUp!"
        );
    }
}

void TestingDriver::validateStartTearDown() {
    this->validate("tearDown");
    auto lastGroup = this->groupStack.back();
    if (lastGroup->hasTearDown) {
        throw runtime_error(
            string("Group '") + lastGroup->description
            + "' already has a tearDown!"
        );
    }
}

void TestingDriver::addGroup(Group* currentGroup,
                             const function<void()> &func) {
    this->validateStartGroup();
    this->groupStack.back()->subGroups.push_back(currentGroup);
    this->groupStack.push_back(currentGroup);
    this->state.push(DriverState::GROUP);
    bool failed;
    try {
        func();
        failed = false;
    } catch(const exception& e) {
        failed = true;
        (*this->log) << "\nAn exception was thrown inside group '"
                     << currentGroup->description
                     << "': "
                     << e.what();
    } catch(...) {
        failed = true;
        (*this->log) << "\nA non-exception object was thrown inside group'"
                     << currentGroup->description
                     << "'";
    }
    if (failed) {
        (*this->log) << ", outside of any test/setUp/tearDown."
                     << "\n\n" << string(105, '*') << "\n"
                     << "* Try to place all your testing code inside setUps, "
                        "tearDowns or tests to ensure each test is executed. *"
                     << "\n" << string(105, '*') << "\n\n";
    }
    this->state.pop();
    this->groupStack.pop_back();
}

void TestingDriver::addTest(Test *currentTest,
                            const function<void()> &func) {
    this->validateStartTest();
    this->groupStack.back()->tests.push_back(currentTest);
    (*this->log) << this->getTestFullName(currentTest->description) << ": ";
    this->executeSetUps(currentTest);
    this->executeTest(currentTest, func);
    this->executeTearDowns(currentTest);
    if (currentTest->failure) {
        (*this->log) << "FAILED\n\t"
                     << currentTest->failure->getMessage()
                     << "\n";
    } else {
        (*this->log) << "PASSED\n";
    }
    for (Group* group: this->groupStack) {
        group->numTests += 1;
        group->numFailedTests += (currentTest->failure != nullptr);
    }
    BaseMatcher::cleanupMatchersCreatedDuringTests();
}

void TestingDriver::addSetUp(const function<void()> &func) {
    this->validateStartSetUp();
    auto lastGroup = this->groupStack.back();
    lastGroup->hasSetUp = true;
    lastGroup->setUpFunc = func;
}

void TestingDriver::addTearDown(const function<void()> &func) {
    this->validateStartTearDown();
    auto lastGroup = this->groupStack.back();
    lastGroup->hasTearDown = true;
    lastGroup->tearDownFunc = func;
}

int TestingDriver::generateTestReport(ostream& report) {
    return this->groupStack[0]->generateTestReport(report, 0u);
}

int TestingDriver::getNumFailedTests() {
    return this->groupStack[0]->numFailedTests;
}

void TestingDriver::validate(const string& methodName) {
    if (this->state.top() == DriverState::TEST) {
        throw runtime_error("Cannot call " + methodName + " within test!");
    }
    if (this->state.top() == DriverState::SET_UP) {
        throw runtime_error("Cannot call " + methodName + " within setUp!");
    }
    if (this->state.top() == DriverState::TEAR_DOWN) {
        throw runtime_error("Cannot call " + methodName + " within tearDown!");
    }
}

string TestingDriver::getTestFullName(const string& testDescription) const {
    string groupStackFullName;
    for (Group* group: this->groupStack) {
        if (group != this->groupStack[0]) {
            if (!groupStackFullName.empty()) {
                groupStackFullName += "::";
            }
            groupStackFullName += group->description;
        }
    }
    if (!groupStackFullName.empty()) {
        groupStackFullName += "::";
    }
    return groupStackFullName + testDescription;
}

void TestingDriver::executeSetUps(Test* currentTest) {
    this->state.push(DriverState::SET_UP);
    for (Group* group: this->groupStack) {
        if (group->hasSetUp) {
            bool failed;
            string failMessage;
            try {
                group->setUpFunc();
                failed = false;
            } catch(const exception& e) {
                failed = true;
                failMessage = "An exception was thrown during the "
                              "setUp of group '" + group->description
                              + "': " + e.what();
            } catch(...) {
                failed = true;
                failMessage = "A non-exception object was thrown during the "
                              "setUp of group '" + group->description + "'.";
            }
            if (failed && currentTest->failure == nullptr) {
                currentTest->failure = new ExpectationFailed(failMessage);
            }
        }
    }
    this->state.pop();
}

void TestingDriver::executeTest(Test* currentTest,
                                const function<void()>& func) {
    this->state.push(DriverState::TEST);
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
    this->state.pop();
}

void TestingDriver::executeTearDowns(Test* currentTest) {
    this->state.push(DriverState::TEAR_DOWN);
    for (int i = (int)this->groupStack.size() - 1; i >= 0; -- i) {
        Group* group = this->groupStack[i];
        if (group->hasTearDown) {
            bool failed;
            string failMessage;
            try {
                group->tearDownFunc();
                failed = false;
            } catch(const exception& e) {
                failed = true;
                failMessage = "An exception was thrown during the "
                              "tearDown of group '" + group->description
                              + "': " + e.what();
            } catch(...) {
                failed = true;
                failMessage = "A non-exception object was thrown during the "
                              "tearDown of group '" + group->description + "'.";
            }
            if (failed && currentTest->failure == nullptr) {
                currentTest->failure = new ExpectationFailed(failMessage);
            }
        }
    }
    this->state.pop();
}

}
