#include "driver.hpp"

using namespace std;

string formatFileName(const char *fileName, const int &lineNumber) {
    if (fileName != nullptr) {
        return " (" + string(fileName) + ":" + to_string(lineNumber) + ")";
    }
    return "";
}

namespace runtime_testing {

TestingDriver::TestingDriver() {
    this->groupStack = {new Group()};
    this->state.push(DriverState::TOP_LEVEL);
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

void TestingDriver::validateStartSetUp(const char* fileName,
                                       const int& lineNumber) {
    this->validate("setUp", fileName, lineNumber);
    auto lastGroup = this->groupStack.back();
    if (lastGroup->hasSetUp) {
        throw std::runtime_error(
            string("Group ") + "'" + lastGroup->description + "'"
            + " already has a setUp!" + formatFileName(fileName, lineNumber)
        );
    }
}

void TestingDriver::validateStartTearDown(const char* fileName,
                                          const int& lineNumber) {
    this->validate("tearDown", fileName, lineNumber);
    auto lastGroup = this->groupStack.back();
    if (lastGroup->hasTearDown) {
        throw std::runtime_error(
            string("Group ") + "'" + lastGroup->description + "'"
            + " already has a tearDown!" + formatFileName(fileName, lineNumber)
        );
    }
}

void TestingDriver::addGroup(Group* currentGroup,
                             const std::function<void()> &func) {
    this->groupStack.back()->subGroups.push_back(currentGroup);
    this->groupStack.push_back(currentGroup);
    this->state.push(DriverState::GROUP);
    func();
    this->state.pop();
    this->groupStack.pop_back();
}

void TestingDriver::addTest(Test *currentTest,
                            const std::function<void()> &func) {
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
    cerr << groupStackFullName << ": ";
    this->state.push(DriverState::TEST);
    try {
        func();
        cerr << "PASSED\n";
    } catch(ExpectationFailed& failure) {
        currentTest->failure = new ExpectationFailed(failure);
        cerr << "FAILED\n\t" << currentTest->failure->getMessage() << "\n";
    }
    this->state.pop();
    this->state.push(DriverState::TEAR_DOWN);
    for (int i = (int)groupStack.size() - 1; i >= 0; -- i) {
        if (groupStack[i]->hasTearDown) {
            groupStack[i]->tearDownFunc();
        }
    }
    this->state.pop();
}

void TestingDriver::addSetUp(const std::function<void()> &func) {
    auto lastGroup = this->groupStack.back();
    lastGroup->hasSetUp = true;
    lastGroup->setUpFunc = func;
}

void TestingDriver::addTearDown(const std::function<void()> &func) {
    auto lastGroup = this->groupStack.back();
    lastGroup->hasTearDown = true;
    lastGroup->tearDownFunc = func;
}

void TestingDriver::generateTestReport(std::ostream& report,
                                       const char* fileName) {
    this->groupStack[0]->generateTestReport(report, fileName);
    delete groupStack[0];
    groupStack.pop_back();
}

int TestingDriver::getNumFailedTests() {
    return this->groupStack[0]->getNumFailedTests();
}

void TestingDriver::validate(
        const string& methodName,
        const char* fileName,
        const int& lineNumber) {
    if (this->state.top() == DriverState::TEST) {
        throw std::runtime_error(
            "Cannot create " + methodName + " within test!"
            + formatFileName(fileName, lineNumber)
        );
    }
    if (this->state.top() == DriverState::SET_UP) {
        throw std::runtime_error(
                "Cannot create " + methodName + " within setUp!"
                + formatFileName(fileName, lineNumber)
        );
    }
    if (this->state.top() == DriverState::TEAR_DOWN) {
        throw std::runtime_error(
                "Cannot create " + methodName + " within tearDown!"
                + formatFileName(fileName, lineNumber)
        );
    }
}

}
