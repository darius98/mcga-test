#include "kktest/extensions/feedback/src/test_logger.hpp"

#include <iomanip>

#include <termcolor/termcolor.hpp>

#include "kktest/core/src/executor.hpp"

using namespace std;

namespace kktest {
namespace feedback {

TestLogger::TestLogger(ostream& stream): stream(stream) {}

void TestLogger::addTest(const ExecutedTest& test) {
    totalTimeTicks += test.getTotalTimeTicks();
    passedTests += test.isPassed();
    failedTests += !test.isPassed();
    failedOptionalTests += (!test.isPassed() && test.isOptional());
    loggedTests += 1;

    printTestMessage(test);
}

void TestLogger::logFinalInformation() {
    stream << "\nTests passed: "
           << termcolor::green
           << passedTests
           << termcolor::reset
           << "\nTests failed: "
           << (failedTests == failedOptionalTests
                    ? (failedTests == 0 ? termcolor::green : termcolor::yellow)
                    : termcolor::red)
           << failedTests
           << termcolor::reset;
    if (failedOptionalTests != 0) {
        stream << " ("
               << termcolor::yellow << failedOptionalTests << termcolor::reset
               << " "
               << (failedOptionalTests == 1 ? "was" : "were")
               << " optional)";
    }
    stream << "\n";
    stream << "Total testing time: "
           << fixed
           << setprecision(3)
           << totalTimeTicks
           << " ticks ("
           << totalTimeTicks * Executor::getTimeTickLengthMs()
           << " ms)\n";
}

void TestLogger::logFatalError(const string& errorMessage) {
    stream << "\nA fatal "
           << termcolor::red
           << "error"
           << termcolor::reset
           << " occurred during execution: "
           << errorMessage
           << "\n";
}

string TestLogger::getRecursiveGroupDescription(GroupPtr group) {
    if (group == nullptr) {
        return "";
    }
    string recursive = getRecursiveGroupDescription(group->getParentGroup());
    if (group->getIndex() == 0 && group->getDescription().empty()) {
        return recursive;
    }
    return recursive + group->getDescription() + "::";
}

void TestLogger::printTestMessage(const ExecutedTest& test) {
    stream << "[";
    if (test.isPassed()) {
        stream << termcolor::green << "P" << termcolor::reset;
    } else if (test.isOptional()) {
        stream << termcolor::yellow << "F" << termcolor::reset;
    } else {
        stream << termcolor::red << "F" << termcolor::reset;
    }
    stream << "] ";
    string groupDescription = getRecursiveGroupDescription(test.getGroup());
    stream << groupDescription
           << test.getDescription()
           << " - "
           << fixed
           << setprecision(3)
           << (test.getNumAttempts() > 1 ? "~ " : "")
           << test.getAvgTimeTicksForExecution()
           << " ticks ("
           << (test.getNumAttempts() > 1 ? "~ " : "")
           << test.getAvgTimeTicksForExecution()
                * Executor::getTimeTickLengthMs()
           << " ms)";
    if (test.getNumAttempts() > 1 && !test.isPassed()) {
        stream << " ("
               << test.getNumPassedAttempts()
               << " passed, "
               << test.getNumRequiredPassedAttempts()
               << " / "
               << test.getNumAttempts()
               << " required)";
    }
    if (test.getNumAttempts() > 1 && test.isPassed()) {
        stream << " ("
               << test.getNumPassedAttempts()
               << " / "
               << test.getNumAttempts()
               << " passed)";
    }
    if (!test.isPassed() && !test.getLastFailure().empty()) {
        stream << "\n\t";
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        string failure = test.getLastFailure();
        while ((pos = failure.find('\n', pos)) != string::npos) {
            failure.replace(pos, 1, "\n\t");
            pos += 2;
        }
        stream << termcolor::red << failure << termcolor::reset;
    }
    stream << "\n";
}

}
}
