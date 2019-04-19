#include "test_logger.hpp"

#include <iomanip>
#include <numeric>

#include <termcolor/termcolor.hpp>

#include "core/time_tick.hpp"

using std::fixed;
using std::milli;
using std::nano;
using std::ostream;
using std::setprecision;
using std::string;
using std::vector;
using termcolor::green;
using termcolor::red;
using termcolor::reset;
using termcolor::yellow;

namespace mcga::test::feedback {

TestLogger::TestLogger(ostream& stream, bool liveLogging)
        : stream(stream), liveLogging(liveLogging) {
}

void TestLogger::onTestExecutionStart(const Test& test) {
    runningTests.insert(test.getId());
    updateVolatileLine(test);
}

void TestLogger::onTestExecutionFinish(const Test& test) {
    runningTests.erase(test.getId());
    updateVolatileLine(test);

    if (!test.isExecuted()) {
        return;
    }

    if (test.getTotalTimeTicks() != -1.0) {
        totalTimeTicks += test.getTotalTimeTicks();
    }
    if (test.isPassed()) {
        ++passedTests;
    } else {
        ++failedTests;
        if (test.isOptional()) {
            ++failedOptionalTests;
        }
    }
    ++loggedTests;

    printTestMessage(test);
}

void TestLogger::printFinalInformation() {
    if (isLastLineVolatile) {
        stream << "\r";
        stream.flush();
    }
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
        stream << " (" << yellow << failedOptionalTests << reset << " "
               << (failedOptionalTests == 1 ? "was" : "were") << " optional)";
    }
    stream << "\n";
    stream << "Total recorded testing time: " << fixed << setprecision(3)
           << totalTimeTicks << " ticks ("
           << TimeTicksToNanoseconds(totalTimeTicks).count() * 1.0 * milli::den
        / nano::den
           << " ms)\n";
    isLastLineVolatile = false;
}

void TestLogger::printWarning(const string& warningMessage) {
    stream << yellow << "Warning: " << warningMessage << reset << "\n";
}

void TestLogger::printTestPassedOrFailedToken(const Test& test) {
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

void TestLogger::printTestAndGroupsDescription(const Test& test) {
    vector<string> groupDescriptions;
    GroupPtr group = test.getGroup();
    while (group != nullptr) {
        string groupDescription = group->getDescription();
        if (!groupDescription.empty()) {
            groupDescriptions.push_back(groupDescription);
        }
        group = group->getParentGroup();
    }
    string groupDescription
      = accumulate(groupDescriptions.rbegin(),
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

void TestLogger::printTestExecutionTime(const Test& test) {
    if (test.getAvgTimeTicksForExecution() != -1.0) {
        stream
          << fixed << setprecision(3) << (test.getNumAttempts() > 1 ? "~ " : "")
          << test.getAvgTimeTicksForExecution() << " ticks ("
          << (test.getNumAttempts() > 1 ? "~ " : "")
          << TimeTicksToNanoseconds(test.getAvgTimeTicksForExecution()).count()
            * 1.0 * milli::den / nano::den
          << " ms)";
    } else {
        stream << "(unknown time)";
    }
}

void TestLogger::printTestAttemptsInfo(const Test& test) {
    stream << "(passed: " << test.getNumPassedAttempts() << "/"
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

void TestLogger::printTestMessage(const Test& test) {
    if (isLastLineVolatile) {
        stream << "\r";
        stream.flush();
    }

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
    isLastLineVolatile = false;
}

void TestLogger::updateVolatileLine(const Test& test) {
    if (!liveLogging) {
        return;
    }

    if (isLastLineVolatile) {
        stream << "\r";
        stream.flush();
    }

    if (runningTests.size() == 1 && *runningTests.begin() == test.getId()) {
        stream << "[" << yellow << "." << reset << "] ";
        printTestAndGroupsDescription(test);
        if (test.getNumAttempts() > 1) {
            stream << " - running attempt " << test.getExecutions().size() + 1
                   << " of " << test.getNumAttempts() << "...";
        } else {
            stream << " - running...";
        }
    } else if (runningTests.size() > 1) {
        stream << runningTests.size() << " tests running...";
    } else if (runningTests.size() == 1) {
        stream << "One test running...";
    } else {
        stream << "No tests running...";
    }
    stream.flush();

    isLastLineVolatile = true;
}

}  // namespace mcga::test::feedback
