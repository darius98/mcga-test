#pragma once

#include <set>

#include "../core/test.hpp"
#include "../core/warning.hpp"

namespace mcga::test {

class StdoutLoggingExtension {
  public:
    StdoutLoggingExtension(bool printSkipped, bool liveLogging);

    void beforeTestExecution(const Test& test);

    void afterTestExecution(const Test& test);

    void onWarning(const Warning& warning);

    void destroy();

  private:
    void clearVolatileLine();

    void printTestStatus(const Test& test);

    void printTestExecutionTime(const Test& test);

    void printTestAttemptsInfo(const Test& test);

    void printTestFailure(const Test::ExecutionInfo& info);

    void printTestMessage(const Test& test);

    void updateVolatileLine(const Test& test);

    std::ostream& stream;

    std::set<int> runningTests;
    bool liveLogging;
    bool printSkipped;
    bool isLastLineVolatile = false;
    double totalTimeTicks = 0;
    int passedTests = 0;
    int skippedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int loggedTests = 0;
};

}  // namespace mcga::test
