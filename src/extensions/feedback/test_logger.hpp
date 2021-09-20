#pragma once

#include <ostream>
#include <set>

#include "core/test.hpp"
#include "core/warning.hpp"

namespace mcga::test::feedback {

class TestLogger {
  public:
    explicit TestLogger(std::ostream& stream, bool liveLogging);

    void onTestExecutionStart(const Test& test);

    void onTestExecutionFinish(const Test& test);

    void printWarning(const Warning& warning);

    void printFinalInformation();

  private:
    void printTestPassedOrFailedToken(const Test& test);

    void printTestAndGroupsDescription(const Test& test);

    void printTestExecutionTime(const Test& test);

    void printTestAttemptsInfo(const Test& test);

    void printTestFailure(const Test::ExecutionInfo& info);

    void printTestMessage(const Test& test);

    void updateVolatileLine(const Test& test);

    std::ostream& stream;

    std::set<int> runningTests;
    bool liveLogging;
    bool isLastLineVolatile = false;
    double totalTimeTicks = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int loggedTests = 0;
};

}  // namespace mcga::test::feedback
