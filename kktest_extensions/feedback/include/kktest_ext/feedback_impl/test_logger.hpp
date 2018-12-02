#ifndef KKTEST_EXTENSIONS_FEEDBACK_INCLUDE_KKTEST_EXT_FEEDBACK_IMPL_TEST_LOGGER_HPP_
#define KKTEST_EXTENSIONS_FEEDBACK_INCLUDE_KKTEST_EXT_FEEDBACK_IMPL_TEST_LOGGER_HPP_

#include <map>
#include <ostream>
#include <set>
#include <utility>

#include <kktest_extension_api.hpp>

namespace kktest {
namespace feedback {

class TestLogger {
 public:
    explicit TestLogger(std::ostream& _stream, bool _maintainTestIndexOrder = true);

    void addGroupInfo(const GroupInfo& groupInfo);

    void logTest(const TestInfo& testInfo);

    void logFinalInformation(bool logNumTests = false);

    void logFatalError(const String& errorMessage, const String& testCaseName = "");

 private:
    String getRecursiveGroupDescription(int groupId);

    String getTestMessage(const TestInfo& testInfo);

    bool isInTerminal() const;

    void modifyOutput(const int& code, std::ostream& streamToChange);

    std::ostream& stream;

    bool maintainTestIndexOrder;
    int testCasesReceived = 0;
    int testCasesFatallyExited = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int testsLogged = 0;
    std::set<std::pair<int, String>> testsQueue;

    std::map<int, GroupInfo> allGroupsInfo;
};

}  // namespace feedback
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_FEEDBACK_INCLUDE_KKTEST_EXT_FEEDBACK_IMPL_TEST_LOGGER_HPP_
