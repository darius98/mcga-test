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
                      const String& testCaseName = "");

    void logTest(const TestInfo& testInfo, const String& testCaseName = "");

    void logFatalError(const String& errorMessage,
                       const String& testCaseName = "");

    void logFinalInformation(bool logNumTests = false);

 private:
    String getRecursiveGroupDescription(int groupId,
                                        const String& testCaseName);

    void printTestMessage(const TestInfo& testInfo, const String& testCaseName);

    std::ostream& stream;

    bool maintainTestIndexOrder;
    int testCasesReceived = 0;
    int testCasesFatallyExited = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int testsLogged = 0;
    std::set<std::pair<TestInfo, String>> testsQueue;

    std::map<std::pair<String, int>, GroupInfo> allGroupsInfo;
};

}
}

#endif
