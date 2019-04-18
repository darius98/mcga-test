#pragma once

#include <map>
#include <ostream>
#include <set>
#include <utility>

#include "core/test.hpp"

namespace mcga::test::feedback {

class TestLogger {
  public:
    explicit TestLogger(std::ostream& stream);

    void addTest(const Test& test);

    void printWarning(const std::string& warningMessage);

    void printFinalInformation();

  private:
    void printTestPassedOrFailedToken(const Test& test);

    void printTestAndGroupsDescription(const Test& test);

    void printTestExecutionTime(const Test& test);

    void printTestAttemptsInfo(const Test& test);

    void printTestFailure(std::string failure);

    void printTestMessage(const Test& test);

    std::ostream& stream;

    double totalTimeTicks = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int loggedTests = 0;
};

}  // namespace mcga::test::feedback
