#include "testing.hpp"

#include "expect.hpp"
#include "group.hpp"

using namespace std;


namespace runtime_testing {


enum UnittestState {
    TOP_LEVEL,
    GROUP,
    TEST,
    SET_UP,
    TEAR_DOWN,
};

Group* globalGroup = nullptr;
vector<Group*> groupStack;
bool testStarted = false;
UnittestState state = UnittestState::TOP_LEVEL;

void initGlobal() {
    if (globalGroup == nullptr) {
        globalGroup = new Group();
        groupStack.push_back(globalGroup);
    }
}

void validate(
        const string& methodName,
        const string& fileName="UNKNOWN_FILE",
        const int& lineNumber=0) {
    if (state == UnittestState::TEST) {
        throw std::runtime_error(
                "Cannot create " + methodName + " within test! "
                + "(" + fileName + ":" + to_string(lineNumber) + ")"
        );
    }
    if (state == UnittestState::SET_UP) {
        throw std::runtime_error(
                "Cannot create " + methodName + " within setUp! "
                + "(" + fileName + ":" + to_string(lineNumber) + ")"
        );
    }
    if (state == UnittestState::TEAR_DOWN) {
        throw std::runtime_error(
                "Cannot create " + methodName + " within tearDown! "
                + "(" + fileName + ":" + to_string(lineNumber) + ")"
        );
    }
}

bool isDuringTest() {
    return state == UnittestState::TEST;
}

void _test(const string& description,
           const function<void()>& testFunc,
           const char* fileName,
           const int& lineNumber) {
    validate("test", fileName, lineNumber);
    initGlobal();
    auto currentTest = new Test();
    currentTest->description = description;
    groupStack.back()->tests.push_back(currentTest);
    string groupStackFullName = fileName + string(":") + to_string(lineNumber);
    UnittestState prevState = state;
    state = UnittestState::SET_UP;
    for (Group* group: groupStack) {
        if (group->hasSetUp) {
            group->setUpFunc();
        }
        if (group != globalGroup) {
            groupStackFullName += " > " + group->description;
        }
    }
    state = prevState;
    groupStackFullName += " > " + currentTest->description;
    cerr << groupStackFullName << ": "; cerr.flush();
    prevState = state;
    state = UnittestState::TEST;
    try {
        testFunc();
        cerr << "PASSED\n";
        cerr.flush();
    } catch(ExpectationFailed& failure) {
        currentTest->failure = new ExpectationFailed(failure);
        cerr << "FAILED\n\t" << currentTest->failure->getMessage();
        cerr.flush();
    }
    state = prevState;
    prevState = state;
    state = UnittestState::TEAR_DOWN;
    for (int i = (int)groupStack.size() - 1; i >= 0; -- i) {
        if (groupStack[i]->hasTearDown) {
            groupStack[i]->tearDownFunc();
        }
    }
    state = prevState;
}

void _setUp(function<void()> setUpFunc,
            const char* fileName,
            const int& lineNumber) {
    validate("setUp", fileName, lineNumber);
    initGlobal();
    auto lastGroup = groupStack.back();
    if (lastGroup->hasSetUp) {
        throw std::runtime_error(
                string("Group ") + "'" + lastGroup->description + "'"
                + " already has a setUp!"
                + " (" + fileName + ":" + to_string(lineNumber) + ")"
        );
    }
    lastGroup->hasSetUp = true;
    lastGroup->setUpFunc = std::move(setUpFunc);
}

void _tearDown(function<void()> tearDownFunc,
               const char* fileName,
               const int& lineNumber) {
    validate("tearDown", fileName, lineNumber);
    initGlobal();
    auto lastGroup = groupStack.back();
    if (lastGroup->hasTearDown) {
        throw std::runtime_error(
                string("Group ")
                + "'" + lastGroup->description + "'"
                + " already has a tearDown!"
                + " (" + fileName + ":" + to_string(lineNumber) + ")"
        );
    }
    lastGroup->hasTearDown = true;
    lastGroup->tearDownFunc = std::move(tearDownFunc);
}

int _getTestReport(const char* fileName) {
    initGlobal();
    stringstream report;
    globalGroup->generateTestReport(report, fileName);
    groupStack.pop_back();
    delete globalGroup;
    globalGroup = nullptr;
    std::cout << report.str();
    return 0;
}

int numFailedTests() {
    return globalGroup->getNumFailedTests();
}

}

void group(const string& description, const function<void()>& groupFunc) {
    using namespace runtime_testing;
    validate("group");
    initGlobal();
    auto currentGroup = new Group();
    currentGroup->description = description;
    groupStack.back()->subGroups.push_back(currentGroup);
    groupStack.push_back(currentGroup);
    UnittestState prevState = state;
    state = UnittestState::GROUP;
    groupFunc();
    state = prevState;
    groupStack.pop_back();
}
