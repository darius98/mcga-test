#pragma once

#include <map>
#include <ostream>
#include <set>
#include <utility>

#include "kktest/core/executed_test.hpp"

namespace mcga::test::feedback {

class TestLogger {
  public:
    explicit TestLogger(std::ostream& stream);

    void addTest(const ExecutedTest& test);

    void printWarning(const std::string& warningMessage);

    void printFinalInformation();

  private:
    void printTestPassedOrFailedToken(const ExecutedTest& test);

    void printTestAndGroupsDescription(const ExecutedTest& test);

    void printTestExecutionTime(const ExecutedTest& test);

    void printTestAttemptsInfo(const ExecutedTest& test);

    void printTestFailure(std::string failure);

    void printTestMessage(const ExecutedTest& test);

    std::ostream& stream;

    double totalTimeTicks = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int loggedTests = 0;
};

}  // namespace mcga::test::feedback
