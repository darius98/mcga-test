#include <unistd.h>

#include <cstdio>

#include <iostream>
#include <sstream>

#include <kktest_ext/feedback_impl/test_logger.hpp>

using std::cout;
using std::ostream;
using std::stringstream;
using std::to_string;

namespace kktest {
namespace feedback {

TestLogger::TestLogger(ostream& _stream, bool _maintainTestIndexOrder):
        stream(_stream),
        maintainTestIndexOrder(_maintainTestIndexOrder),
        isInTerminal(stream.rdbuf() == cout.rdbuf() && isatty(fileno(stdout)) != 0) {}

void TestLogger::addGroupInfo(const GroupInfo& groupInfo) {
    allGroupsInfo[groupInfo.index] = groupInfo;
}

void TestLogger::logTest(const TestInfo& testInfo) {
    testsQueue.insert(testInfo);
    passedTests += testInfo.passed;
    failedTests += !testInfo.passed;
    failedOptionalTests += !testInfo.passed && testInfo.optional;
    testCasesReceived += (testInfo.index == 1);
    while (!testsQueue.empty() &&
                (testsQueue.begin()->index == testsLogged + 1 || !maintainTestIndexOrder)) {
        printTestMessage(*testsQueue.begin());
        testsQueue.erase(testsQueue.begin());
        testsLogged += 1;
    }
}

void TestLogger::logFinalInformation(bool logNumTests) {
    stream << "\n";
    if (logNumTests) {
        if (testCasesFatallyExited != 0) {
            stream << red("Warning: some test cases closed unexpectedly!") << "\n\n";
            stream << "Test cases found: " << testCasesReceived << "\n"
                   << "Test cases executed successfully: "
                   << testCasesReceived - testCasesFatallyExited << "\n"
                   << "Test cases executed with fatal errors: "
                   << red(to_string(testCasesFatallyExited))
                   << "\n";
        }
        stream << "Total tests executed: " << passedTests + failedTests << "\n";
    }
    stream << "Tests passed: " << green(to_string(passedTests)) << "\n";
    stream << "Tests failed: " << colored(
        to_string(failedTests),
        (failedTests == failedOptionalTests ? (failedTests == 0 ? GREEN : YELLOW) : RED));
    if (failedOptionalTests) {
        stream << " ("
               << yellow(to_string(failedOptionalTests))
               << " "
               << (failedOptionalTests == 1 ? "was" : "were")
               << " optional)";
    }
    stream << "\n";
}

void TestLogger::logFatalError(const String& errorMessage, const String& testCaseName) {
    testCasesFatallyExited += 1;
    stream << "\nA fatal " << red("error") << " occurred during execution";
    if (!testCaseName.empty()) {
        stream << " of test case " << testCaseName;
    }
    stream << ": " << errorMessage << "\n";
}

String TestLogger::getRecursiveGroupDescription(int groupId) {
    auto groupInfoIterator = allGroupsInfo.find(groupId);
    if (groupInfoIterator == allGroupsInfo.end()) {
        return "";
    }
    GroupInfo groupInfo = groupInfoIterator->second;
    String recursive = getRecursiveGroupDescription(groupInfo.parentGroupIndex);
    if (groupInfo.index == 0 && groupInfo.description.empty()) {
        return recursive;
    }
    return recursive + groupInfo.description + "::";
}

void TestLogger::printTestMessage(const TestInfo& testInfo) {
    stream << "[" << colored(testInfo.passed ? "P" : "F",
                             testInfo.passed ? GREEN : (testInfo.optional ? YELLOW : RED)) << "] ";
    String groupDescription = getRecursiveGroupDescription(testInfo.groupIndex);
    if (!groupDescription.empty()) {
        stream << grey(groupDescription);
    }
    stream << testInfo.description;
    if (!testInfo.passed) {
        stream << "\n";
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        String failureMessage = testInfo.failureMessage;
        while ((pos = failureMessage.find('\n', pos)) != String::npos) {
            failureMessage.replace(pos, 1, "\n\t");
            pos += 2;
        }
        stream << "\t" << failureMessage;
    }
    stream << "\n";
}

String TestLogger::colored(const String& message, Color color) {
    if (isInTerminal) {
        return "\x1b[" + to_string(color) + "m" + message + "\x1b[0m";
    }
    return message;
}

String TestLogger::red(const String& message) {
    return colored(message, RED);
}

String TestLogger::yellow(const String& message) {
    return colored(message, YELLOW);
}

String TestLogger::green(const String& message) {
    return colored(message, GREEN);
}

String TestLogger::grey(const String& message) {
    return colored(message, GREY);
}

}  // namespace feedback
}  // namespace kktest
