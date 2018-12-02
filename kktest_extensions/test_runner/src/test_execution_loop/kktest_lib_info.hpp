#ifndef KKTEST_EXT_TEST_RUNNER_TEST_EXECUTION_LOOP_KKTEST_LIB_INFO_H_
#define KKTEST_EXT_TEST_RUNNER_TEST_EXECUTION_LOOP_KKTEST_LIB_INFO_H_

#include <map>
#include <vector>

#include <kktest_extension_api>

namespace kktest {
namespace test_runner {

struct KKTestCaseInfo {
    String testExecutablePath;
    std::vector<GroupInfo> groupsReceived;
    std::vector<TestInfo> testsReceived;
    bool finished = false;
    String errorMessage;

    enum LastReceived { TEST, GROUP, FINISH, FINISH_WITH_ERROR } lastReceived;
};

}
}

#endif
