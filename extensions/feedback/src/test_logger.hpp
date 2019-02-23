#ifndef KKTEST_EXTENSIONS_FEEDBACK_SRC_TEST_LOGGER_HPP_
#define KKTEST_EXTENSIONS_FEEDBACK_SRC_TEST_LOGGER_HPP_

#include <map>
#include <ostream>
#include <set>
#include <utility>

#include "core/src/group.hpp"
#include "core/src/test.hpp"

namespace kktest {
namespace feedback {

class TestLogger {
 public:
    explicit TestLogger(std::ostream& stream);

    void logTest(const ExecutedTest& test);

    void logFatalError(const std::string& errorMessage);

    void logFinalInformation();

 private:
    std::string getRecursiveGroupDescription(GroupPtr group);

    void printTestMessage(const ExecutedTest& test);

    std::ostream& stream;

    double totalTimeTicks = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int testsLogged = 0;
};

}
}

#endif
