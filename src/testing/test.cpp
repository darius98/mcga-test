#include "test.hpp"

#include "expect.hpp"

using namespace std;


namespace runtime_testing {

struct Test {
    string description;

    ExpectationFailed* failure = nullptr;
};

struct Group {
    string description;

    bool hasSetUp = false;
    function<void()> setUp;

    bool hasTearDown = false;
    function<void()> tearDown;

    bool startedTests = false;

    vector<Group*> subGroups;
    vector<Test*> tests;

    void generateTestReport(
            stringstream& report,
            const std::string& currentGroupFullName=__FILENAME__) {
        for (Test* test: this->tests) {
            report << currentGroupFullName
                   << " > "
                   << test->description
                   << ": "
                   << (test->failure != nullptr ? "FAILED" : "PASSED")
                   << "\n";
            if (test->failure != nullptr) {
                report << "\t" << test->failure->getMessage() << "\n";
            }
            delete test;
        }
        for (Group* group: this->subGroups) {
            group->generateTestReport(
                    report,
                    currentGroupFullName + " > " + group->description
            );
            delete group;
        }
    }
};

Group* globalGroup = nullptr;
vector<Group*> groupStack;
bool testStarted = false;

void initGlobal() {
    if (globalGroup == nullptr) {
        globalGroup = new Group();
        groupStack.push_back(globalGroup);
    }
}

bool isDuringTest() {
    return testStarted;
}

void test(const string& description, function<void()> testFunc) {
    initGlobal();
    auto currentTest = new Test();
    currentTest->description = description;
    groupStack.back()->tests.push_back(currentTest);
    groupStack.back()->startedTests = true;
    for (Group* group: groupStack) {
        if (group->hasSetUp) {
            group->setUp();
        }
    }
    testStarted = true;
    try {
        testFunc();
    } catch(ExpectationFailed& failure) {
        currentTest->failure = new ExpectationFailed(failure);
    }
    testStarted = false;
    for (int i = (int)groupStack.size() - 1; i >= 0; -- i) {
        if (groupStack[i]->hasTearDown) {
            groupStack[i]->tearDown();
        }
    }
}

void group(const string& description, function<void()> groupFunc) {
    initGlobal();
    auto currentGroup = new Group();
    currentGroup->description = description;
    groupStack.back()->subGroups.push_back(currentGroup);
    groupStack.back()->startedTests = true;
    groupStack.push_back(currentGroup);
    groupFunc();
    groupStack.pop_back();
}

void setUp(function<void()> setUpFunc) {
    initGlobal();
    auto lastGroup = groupStack.back();
    if (lastGroup->hasSetUp) {
        throw std::runtime_error(
                "Group '" + lastGroup->description + "' already has a setUp!"
        );
    }
    lastGroup->hasSetUp = true;
    lastGroup->setUp = std::move(setUpFunc);
}

void tearDown(function<void()> tearDownFunc) {
    initGlobal();
    auto lastGroup = groupStack.back();
    if (lastGroup->hasTearDown) {
        throw std::runtime_error(
                "Group '" + lastGroup->description + "' already has a tearDown!"
        );
    }
    lastGroup->hasTearDown = true;
    lastGroup->tearDown = std::move(tearDownFunc);
}

int getTestReport() {
    initGlobal();
    stringstream report;
    globalGroup->generateTestReport(report);
    groupStack.pop_back();
    delete globalGroup;
    globalGroup = nullptr;
    std::cout << report.str();
    return 0;
}

}