#ifndef KKTEST_EXT_FEEDBACK_IMPL_TEST_LOGGER_H_
#define KKTEST_EXT_FEEDBACK_IMPL_TEST_LOGGER_H_

#include <map>
#include <ostream>
#include <set>
#include <string>

#include <kktest_extension_api>

namespace kktest {
namespace feedback {

class TestLogger {
public:
    explicit TestLogger(std::ostream& _stream, bool _maintainTestIndexOrder=true);

    void addGroupInfo(const GroupInfo& groupInfo);

    void logTest(const TestInfo& testInfo);

    void logFinalInformation(bool logNumTests=false);

    void logFatalError(const std::string& errorMessage, const std::string& testCaseName="");

private:
    std::string getRecursiveGroupDescription(int groupId);

    std::string getTestMessage(const TestInfo& testInfo);

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
    std::set<std::pair<int, std::string>> testsQueue;

    std::map<int, GroupInfo> allGroupsInfo;
};

}
}

#endif
