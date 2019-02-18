#include "extensions/feedback/src/test_logger.hpp"

#include <termcolor/termcolor.hpp>

using namespace std;

namespace kktest {
namespace feedback {

TestLogger::TestLogger(ostream& _stream): stream(_stream) {}

void TestLogger::logTest(TestRun testRun, const string& testCaseName) {
    passedTests += testRun.isPassed();
    failedTests += !testRun.isPassed();

    failedOptionalTests +=
            (!testRun.isPassed() && testRun.getTest()->isOptional());
    testCasesReceived += (testRun.getTest()->getIndex() == 1);
    printTestMessage(testRun, testCaseName);
    testsLogged += 1;
}

void TestLogger::logFinalInformation(bool logNumTests) {
    stream << "\n";
    if (logNumTests) {
        if (testCasesFatallyExited != 0) {
            stream << termcolor::red
                   << "Warning: some test cases closed unexpectedly!"
                   << termcolor::reset
                   << "\n\n"
                   << "Test cases found: "
                   << testCasesReceived
                   << "\n"
                   << "Test cases executed successfully: "
                   << testCasesReceived - testCasesFatallyExited
                   << "\n"
                   << "Test cases executed with fatal errors: "
                   << termcolor::red
                   << testCasesFatallyExited
                   << termcolor::reset
                   << "\n";
        }
        stream << "Total tests executed: " << passedTests + failedTests << "\n";
    }
    stream << "Tests passed: "
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
}

void TestLogger::logFatalError(const string& errorMessage,
                               const string& testCaseName) {
    testCasesFatallyExited += 1;
    stream << "\nA fatal "
           << termcolor::red
           << "error"
           << termcolor::reset
           << " occurred during execution";
    if (!testCaseName.empty()) {
        stream << " of test case " << testCaseName;
    }
    stream << ": " << errorMessage << "\n";
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

void TestLogger::printTestMessage(TestRun testRun, const string& testCaseName) {
    stream << "[";
    if (testRun.isPassed()) {
        stream << termcolor::green << "P" << termcolor::reset;
    } else {
        stream << termcolor::red << "F" << termcolor::reset;
    }
    stream << "] ";
    string groupDescription = getRecursiveGroupDescription(
            testRun.getTest()->getGroup());
    stream << termcolor::grey
           << groupDescription
           << termcolor::reset
           << testRun.getTest()->getDescription();
    if (!testRun.isPassed()) {
        stream << "\n\t";
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        string failure = testRun.getFailure();
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
