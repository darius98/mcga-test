#ifndef KKTEST_EXTENSIONS_FEEDBACK_SRC_TEST_LOGGER_HPP_
#define KKTEST_EXTENSIONS_FEEDBACK_SRC_TEST_LOGGER_HPP_

#include <map>
#include <ostream>
#include <set>
#include <utility>

#include "core/src/group.hpp"
#include "core/src/test_run.hpp"

namespace kktest {
namespace feedback {

class TestLogger {
 public:
    explicit TestLogger(std::ostream& _stream);

    void logTest(TestRun testRun, const std::string& testCaseName = "");

    void logFatalError(const std::string& errorMessage,
                       const std::string& testCaseName = "");

    void logFinalInformation(bool logNumTests = false);

 private:
    std::string getRecursiveGroupDescription(GroupPtr group);

    void printTestMessage(TestRun testRun, const std::string& testCaseName);

    std::ostream& stream;

    int testCasesReceived = 0;
    int testCasesFatallyExited = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int testsLogged = 0;
};

}
}

#endif
