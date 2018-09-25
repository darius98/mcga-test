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
    func();
    this->state.pop();
    this->groupStack.pop_back();
}

void TestingDriver::addTest(Test *currentTest,
                            const function<void()> &func) {
    this->validateStartTest();
    groupStack.back()->tests.push_back(currentTest);
    string groupStackFullName;
    this->state.push(DriverState::SET_UP);
    for (Group* group: groupStack) {
        if (group->hasSetUp) {
            group->setUpFunc();
        }
        if (group != groupStack[0]) {
            if (!groupStackFullName.empty()) {
                groupStackFullName += "::";
            }
            groupStackFullName += group->description;
        }
    }
    this->state.pop();
    if (!groupStackFullName.empty()) {
        groupStackFullName += "::";
    }
    groupStackFullName += currentTest->description;
    (*this->log) << groupStackFullName << ": ";
    this->state.push(DriverState::TEST);
    try {
        func();
        (*this->log) << "PASSED\n";
    } catch(ExpectationFailed& failure) {
        currentTest->failure = new ExpectationFailed(failure);
        (*this->log) << "FAILED\n\t"
                     << currentTest->failure->getMessage()
                     << "\n";
    }
    BaseMatcher::cleanup();
    this->state.pop();
    this->state.push(DriverState::TEAR_DOWN);
    for (int i = (int)this->groupStack.size() - 1; i >= 0; -- i) {
        this->groupStack[i]->numTests += 1;
        this->groupStack[i]->numFailedTests += currentTest->failure != nullptr;
        if (this->groupStack[i]->hasTearDown) {
            this->groupStack[i]->tearDownFunc();
        }
    }
    this->state.pop();
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

}
