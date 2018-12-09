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
    explicit TestLogger(std::ostream& _stream,
                        bool _maintainTestIndexOrder = true);

    void addGroupInfo(const GroupInfo& groupInfo,
                      const String& testCaseName = "");

    void logTest(const TestInfo& testInfo, const String& testCaseName = "");

    void logFatalError(const String& errorMessage,
                       const String& testCaseName = "");

    void logFinalInformation(bool logNumTests = false);

 private:
    typedef int Color;
    static constexpr const Color RED = 31;
    static constexpr const Color GREEN = 32;
    static constexpr const Color YELLOW = 33;
    static constexpr const Color GREY = 90;

    String getRecursiveGroupDescription(int groupId,
                                        const String& testCaseName);

    void printTestMessage(const TestInfo& testInfo, const String& testCaseName);

    String colored(const String& message, Color color);
    String red(const String& s);
    String yellow(const String& s);
    String green(const String& s);
    String grey(const String& s);

    std::ostream& stream;

    bool isInTerminal;
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

}  // namespace feedback
}  // namespace kktest

#endif  // KKTEST_EXTENSIONS_FEEDBACK_INCLUDE_KKTEST_EXT_FEEDBACK_IMPL_TEST_LOGGER_HPP_
