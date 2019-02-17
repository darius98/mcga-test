#include "extensions/feedback/src/test_logger.hpp"

#include <termcolor/termcolor.hpp>

using namespace std;

namespace kktest {
namespace feedback {

TestLogger::TestLogger(ostream& _stream, bool _maintainTestIndexOrder):
        stream(_stream),
        maintainTestIndexOrder(_maintainTestIndexOrder) {}

void TestLogger::addGroupInfo(const GroupInfo& groupInfo,
                              const string& testCaseName) {
    allGroupsInfo[{testCaseName, groupInfo.index}] = groupInfo;
}

void TestLogger::logTest(const TestInfo& testInfo, const string& testCaseName) {
    testsQueue.insert({testInfo, testCaseName});
    passedTests += testInfo.passed;
    failedTests += !testInfo.passed;
    failedOptionalTests += !testInfo.passed && testInfo.optional;
    testCasesReceived += (testInfo.index == 1);
    while (!testsQueue.empty() &&
                (testsQueue.begin()->first.index == testsLogged + 1 ||
                 !maintainTestIndexOrder)) {
        printTestMessage(testsQueue.begin()->first, testsQueue.begin()->second);
        testsQueue.erase(testsQueue.begin());
        testsLogged += 1;
    }
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

string TestLogger::getRecursiveGroupDescription(int groupId,
                                                const string& testCaseName) {
    auto groupInfoIterator = allGroupsInfo.find({testCaseName, groupId});
    if (groupInfoIterator == allGroupsInfo.end()) {
        return "";
    }
    GroupInfo groupInfo = groupInfoIterator->second;
    string recursive = getRecursiveGroupDescription(groupInfo.parentGroupIndex,
                                                    testCaseName);
    if (groupInfo.index == 0 && groupInfo.description.empty()) {
        return recursive;
    }
    return recursive + groupInfo.description + "::";
}

void TestLogger::printTestMessage(const TestInfo& testInfo,
                                  const string& testCaseName) {
    stream << "[";
    if (testInfo.passed) {
        stream << termcolor::green << "P" << termcolor::reset;
    } else {
        stream << termcolor::red << "F" << termcolor::reset;
    }
    stream << "] ";
    string groupDescription = getRecursiveGroupDescription(testInfo.groupIndex,
                                                           testCaseName);
    stream << termcolor::grey
           << groupDescription
           << termcolor::reset
           << testInfo.description;
    if (!testInfo.passed) {
        stream << "\n\t";
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        string failureMessage = testInfo.failureMessage;
        while ((pos = failureMessage.find('\n', pos)) != string::npos) {
            failureMessage.replace(pos, 1, "\n\t");
            pos += 2;
        }
        stream << termcolor::red << failureMessage << termcolor::reset;
    }
    stream << "\n";
}

}
}
