#ifndef KKTEST_EXTENSIONS_FEEDBACK_SRC_TEST_LOGGER_HPP_
#define KKTEST_EXTENSIONS_FEEDBACK_SRC_TEST_LOGGER_HPP_

#include <map>
#include <ostream>
#include <set>
#include <utility>

#include "core/src/info.hpp"

namespace kktest {
namespace feedback {

class TestLogger {
 public:
    explicit TestLogger(std::ostream& _stream,
                        bool _maintainTestIndexOrder = true);

    void addGroupInfo(const GroupInfo& groupInfo,
                      const std::string& testCaseName = "");

    void logTest(const TestInfo& testInfo,
                 const std::string& testCaseName = "");

    void logFatalError(const std::string& errorMessage,
                       const std::string& testCaseName = "");

    void logFinalInformation(bool logNumTests = false);

 private:
    std::string getRecursiveGroupDescription(int groupId,
                                             const std::string& testCaseName);

    void printTestMessage(const TestInfo& testInfo,
                          const std::string& testCaseName);

    std::ostream& stream;

    bool maintainTestIndexOrder;
    int testCasesReceived = 0;
    int testCasesFatallyExited = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int testsLogged = 0;
    std::set<std::pair<TestInfo, std::string>> testsQueue;

    std::map<std::pair<std::string, int>, GroupInfo> allGroupsInfo;
};

}
}

#endif
