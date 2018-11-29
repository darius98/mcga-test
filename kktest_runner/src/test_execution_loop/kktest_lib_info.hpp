#ifndef KKTEST_RUNNER_TEST_EXECUTION_LOOP_KKTEST_LIB_INFO_H_
#define KKTEST_RUNNER_TEST_EXECUTION_LOOP_KKTEST_LIB_INFO_H_

#include <map>
#include <string>
#include <vector>

#include <kktest_extension_api>

namespace runner {

struct KKTestCaseInfo {
    std::string testExecutablePath;
    std::vector<kktest::GroupInfo> groupsReceived;
    std::vector<kktest::TestInfo> testsReceived;
    bool finished = false;
    std::string errorMessage;

    enum LastReceived { TEST, GROUP, FINISH, FINISH_WITH_ERROR } lastReceived;
};

}

#endif
