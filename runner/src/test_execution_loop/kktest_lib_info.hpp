#ifndef KKTEST_RUNNER_TEST_EXECUTION_LOOP_KKTEST_LIB_INFO_H_
#define KKTEST_RUNNER_TEST_EXECUTION_LOOP_KKTEST_LIB_INFO_H_

#include <map>
#include <string>
#include <vector>


namespace runner {

struct TestInfo {
    int groupIndex;
    int index;
    int line;
    std::string file;
    bool optional;
    std::string description;
    bool passed;
    std::string failureMessage;
};

struct GroupInfo {
    int parentGroupIndex;
    int index;
    int line;
    std::string file;
    std::string description;
};

struct KKTestCaseInfo {
    std::string testExecutablePath;
    std::map<int, GroupInfo> groups;
    std::vector<TestInfo> tests;
    bool finished = false;
    bool finishedWithError = false;
    std::string errorMessage;

    std::string getRecursiveGroupDescription(int groupId) const;
};

}

#endif
