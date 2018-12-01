#include <cstdio>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include <kktest_ext/feedback_impl/test_logger.hpp>

using std::cout;
using std::ostream;
using std::string;
using std::stringstream;

namespace kktest {
namespace feedback {

TestLogger::TestLogger(ostream& _stream, bool _maintainTestIndexOrder):
        stream(_stream), maintainTestIndexOrder(_maintainTestIndexOrder) {}

void TestLogger::addGroupInfo(const GroupInfo& groupInfo) {
    allGroupsInfo[groupInfo.index] = groupInfo;
}

void TestLogger::logTest(const TestInfo& testInfo) {
    testsQueue.insert({
        testInfo.index,
        getTestMessage(testInfo)
    });
    passedTests += testInfo.passed;
    failedTests += !testInfo.passed;
    failedOptionalTests += !testInfo.passed && testInfo.optional;
    testCasesReceived += (testInfo.index == 1);
    while (!testsQueue.empty() &&
                (testsQueue.begin()->first == testsLogged + 1 || !maintainTestIndexOrder)) {
        stream << testsQueue.begin()->second;
        testsQueue.erase(testsQueue.begin());
        testsLogged += 1;
    }
}

void TestLogger::logFinalInformation(bool logNumTests) {
    stream << "\n";
    if (logNumTests) {
        if (testCasesFatallyExited != 0) {
            modifyOutput(31, stream);
            stream << "Warning: some test cases closed unexpectedly!\n\n";
            modifyOutput(0, stream);
            stream << "Test cases found: " << testCasesReceived << "\n"
                   << "Test cases executed successfully: "
                   << testCasesReceived - testCasesFatallyExited << "\n"
                   << "Test cases executed with fatal errors: ";
            modifyOutput(31, stream);
            stream << testCasesFatallyExited;
            modifyOutput(0, stream);
            stream << "\n";
        }
        stream << "Total tests executed: " << passedTests + failedTests << "\n";
    }
    stream << "Tests passed: ";
    modifyOutput(32, stream);
    stream << passedTests;
    modifyOutput(0, stream);
    stream << "\nTests failed: ";
    modifyOutput((failedTests == failedOptionalTests ? (failedTests == 0 ? 32 : 33) : 31), stream);
    stream << failedTests;
    modifyOutput(0, stream);
    if (failedOptionalTests) {
        stream << " (";
        modifyOutput(33, stream);
        stream << failedOptionalTests;
        modifyOutput(0, stream);
        stream << " " << (failedOptionalTests == 1 ? "was" : "were") << " optional)";
    }
    stream << "\n";
}

void TestLogger::logFatalError(const string& errorMessage, const string& testCaseName) {
    testCasesFatallyExited += 1;
    stream << "\nA fatal ";
    modifyOutput(31, stream);
    stream << "error";
    modifyOutput(0, stream);
    stream << " occurred during execution";
    if (!testCaseName.empty()) {
        stream << " of test case " << testCaseName;
    }
    stream << ": " << errorMessage << "\n";
}

string TestLogger::getRecursiveGroupDescription(int groupId) {
    auto groupInfoIterator = allGroupsInfo.find(groupId);
    if (groupInfoIterator == allGroupsInfo.end()) {
        return "";
    }
    GroupInfo groupInfo = groupInfoIterator->second;
    return getRecursiveGroupDescription(groupInfo.parentGroupIndex) + groupInfo.description + "::";
}

string TestLogger::getTestMessage(const TestInfo& testInfo) {
    stringstream sBuffer;
    sBuffer << "[";
    modifyOutput(testInfo.passed ? 32 : (testInfo.optional ? 33 : 31), sBuffer);
    sBuffer << (testInfo.passed ? 'P' : 'F');
    modifyOutput(0, sBuffer);
    sBuffer << "] ";
    string groupDescription = getRecursiveGroupDescription(testInfo.groupIndex);
    if (!groupDescription.empty()) {
        modifyOutput(90, sBuffer);
        sBuffer << groupDescription;
        modifyOutput(0, sBuffer);
    }
    sBuffer << testInfo.description;
    if (!testInfo.passed) {
        sBuffer << "\n";
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        string failureMessage = testInfo.failureMessage;
        while ((pos = failureMessage.find('\n', pos)) != string::npos) {
            failureMessage.replace(pos, 1, "\n\t");
            pos += 2;
        }
        sBuffer << "\t" << failureMessage;
    }
    sBuffer << "\n";
    return sBuffer.str();
}

bool TestLogger::isInTerminal() const {
    return stream.rdbuf() == cout.rdbuf() && isatty(fileno(stdout)) != 0;
}

void TestLogger::modifyOutput(const int& code, ostream& streamToChange) {
    if (!isInTerminal()) {
        return;
    }
    streamToChange << "\x1b[" << code << "m";
}

}
}
