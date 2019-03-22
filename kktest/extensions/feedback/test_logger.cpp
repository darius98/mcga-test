#include "kktest/extensions/feedback/test_logger.hpp"

#include <numeric>
#include <iomanip>

#include <termcolor/termcolor.hpp>

#include "kktest/core/time_tick.hpp"

using namespace std;
using namespace termcolor;

namespace kktest::feedback {

TestLogger::TestLogger(ostream& stream): stream(stream) {}

void TestLogger::addTest(const ExecutedTest& test) {
    if (test.getTotalTimeTicks() != -1.0) {
        totalTimeTicks += test.getTotalTimeTicks();
    }
    passedTests += test.isPassed();
    failedTests += !test.isPassed();
    failedOptionalTests += (!test.isPassed() && test.isOptional());
    loggedTests += 1;

    printTestMessage(test);
}

void TestLogger::printFinalInformation() {
    stream << "\n";
    stream << "Tests passed: " << green << passedTests << reset << "\n";
    stream << "Tests failed: ";
    if (failedTests == 0) {
        stream << green << failedTests << reset;
    } else if (failedTests == failedOptionalTests) {
        stream << yellow << failedTests << reset;
    } else {
        stream << red << failedTests << reset;
    }
    if (failedOptionalTests != 0) {
        stream << " ("
               << yellow << failedOptionalTests << reset
               << " "
               << (failedOptionalTests == 1 ? "was" : "were")
               << " optional)";
    }
    stream << "\n";
    stream << "Total recorded testing time: "
           << fixed
           << setprecision(3)
           << totalTimeTicks
           << " ticks ("
           << TimeTicksToNanoseconds(totalTimeTicks).count() * 0.000001
           << " ms)\n";
}

void TestLogger::printWarning(const string& warningMessage) {
    stream << yellow << "Warning: " << warningMessage << reset << "\n";
}

void TestLogger::printTestPassedOrFailedToken(const ExecutedTest& test) {
    stream << "[";
    if (test.isPassed()) {
        stream << green << "P" << reset;
    } else if (test.isOptional()) {
        stream << yellow << "F" << reset;
    } else {
        stream << red << "F" << reset;
    }
    stream << "]";
}

void TestLogger::printTestAndGroupsDescription(const ExecutedTest& test) {
    vector<string> groupDescriptions;
    GroupPtr group = test.getGroup();
    while (group != nullptr) {
        string groupDescription = group->getDescription();
        if (!groupDescription.empty()) {
            groupDescriptions.push_back(groupDescription);
        }
        group = group->getParentGroup();
    }
    string groupDescription = accumulate(groupDescriptions.rbegin(),
                                         groupDescriptions.rend(),
                                         string(""),
                                         [](const string& a, const string& b) {
        return a.empty() ? b : (a + "::" + b);
    });
    if (!groupDescription.empty()) {
        groupDescription += "::";
    }
    stream << groupDescription << test.getDescription();
}

void TestLogger::printTestExecutionTime(const ExecutedTest& test) {
    if (test.getAvgTimeTicksForExecution() != -1.0) {
        stream << fixed
               << setprecision(3)
               << (test.getNumAttempts() > 1 ? "~ " : "")
               << test.getAvgTimeTicksForExecution()
               << " ticks ("
               << (test.getNumAttempts() > 1 ? "~ " : "")
               << TimeTicksToNanoseconds(
                      test.getAvgTimeTicksForExecution()).count() * 0.000001
               << " ms)";
    } else {
        stream << "(unknown time)";
    }
}

void TestLogger::printTestAttemptsInfo(const ExecutedTest& test) {
    stream << "(passed: "
           << test.getNumPassedAttempts()
           << "/"
           << test.getNumAttempts();
    if (!test.isPassed()) {
        stream << ", required: " << test.getNumRequiredPassedAttempts();
    }
    stream << ")";
}

void TestLogger::printTestFailure(string failure) {
    stream << "\n\t";
    // TODO(darius98): This should be somewhere else (in utils maybe?)
    size_t pos = 0;
    while ((pos = failure.find('\n', pos)) != string::npos) {
        failure.replace(pos, 1, "\n\t");
        pos += 2;
    }
    stream << red << failure << reset;
}

void TestLogger::printTestMessage(const ExecutedTest& test) {
    printTestPassedOrFailedToken(test);
    stream << " ";
    printTestAndGroupsDescription(test);
    stream << " - ";
    printTestExecutionTime(test);
    if (test.getNumAttempts() > 1) {
        stream << ' ';
        printTestAttemptsInfo(test);
    }
    if (!test.isPassed() && !test.getLastFailure().empty()) {
        printTestFailure(test.getLastFailure());
    }
    stream << "\n";
}

}
