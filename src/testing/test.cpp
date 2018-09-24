#include "test.hpp"

#include "expect.hpp"

using namespace std;


namespace runtime_testing {

struct Test {
    string description;

    ExpectationFailed* failure = nullptr;

    void print(ostream& report, const std::string& currentGroupFullName) {
        report << currentGroupFullName
               << " > "
               << this->description
               << ": "
               << (this->failure != nullptr ? "FAILED" : "PASSED")
               << "\n";
        if (this->failure != nullptr) {
            report << "\t" << this->failure->getMessage() << "\n";
        }
    }
};

struct Group {
    string description;

    bool hasSetUp = false;
    function<void()> setUpFunc;

    bool hasTearDown = false;
    function<void()> tearDownFunc;

    vector<Group*> subGroups;
    vector<Test*> tests;

    void generateTestReport(
            stringstream& report,
            const std::string& currentGroupFullName=__FILENAME__) {
        for (Test* test: this->tests) {
            test->print(report, currentGroupFullName);
            delete test->failure;
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

    int getNumFailedTests() {
        int num = 0;
        for (Test* test: this->tests) {
            num += (test->failure != nullptr);
        }
        for (Group* group: this->subGroups) {
            num += group->getNumFailedTests();
        }
        return num;
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

void _test(const string& description,
           function<void()> testFunc,
           const char* fileName,
           const int& lineNumber) {
    initGlobal();
    auto currentTest = new Test();
    currentTest->description = description;
    groupStack.back()->tests.push_back(currentTest);
    string groupStackFullName = fileName + string(":") + to_string(lineNumber);
    for (Group* group: groupStack) {
        if (group->hasSetUp) {
            group->setUpFunc();
        }
        if (group != globalGroup) {
            groupStackFullName += " > " + group->description;
        }
    }
    groupStackFullName += " > " + currentTest->description;
    cerr << groupStackFullName << ": "; cerr.flush();
    testStarted = true;
    try {
        testFunc();
        cerr << "PASSED\n";
        cerr.flush();
    } catch(ExpectationFailed& failure) {
        currentTest->failure = new ExpectationFailed(failure);
        cerr << "FAILED\n\t" << currentTest->failure->getMessage();
        cerr.flush();
    }
    testStarted = false;
    for (int i = (int)groupStack.size() - 1; i >= 0; -- i) {
        if (groupStack[i]->hasTearDown) {
            groupStack[i]->tearDownFunc();
        }
    }
}

void _group(const string& description,
            const function<void()>& groupFunc,
            const char* fileName,
            const int& lineNumber) {
    initGlobal();
    auto currentGroup = new Group();
    currentGroup->description = description;
    groupStack.back()->subGroups.push_back(currentGroup);
    groupStack.push_back(currentGroup);
    groupFunc();
    groupStack.pop_back();
}

void _setUp(function<void()> setUpFunc,
            const char* fileName,
            const int& lineNumber) {
    initGlobal();
    auto lastGroup = groupStack.back();
    if (lastGroup->hasSetUp) {
        throw std::runtime_error(
                string("Group ")
                + "'" + lastGroup->description + "'"
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

int numFailedTests() {
    return globalGroup->getNumFailedTests();
}

}