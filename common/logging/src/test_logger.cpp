#include <cstdio>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "test_logger.hpp"

using namespace std;

namespace logging {

TestLogger::TestLogger(ostream& _stream, bool _maintainTestIndexOrder):
        stream(_stream), maintainTestIndexOrder(_maintainTestIndexOrder) {}

void TestLogger::logTest(int testIndex,
                         const string& groupDescription,
                         const string& description,
                         bool isOptional,
                         bool isPassed,
                         const string& failureMessage) {
    testsQueue.insert({
        testIndex,
        getTestMessage(groupDescription, description, isOptional, isPassed, failureMessage)
    });
    passedTests += isPassed;
    failedTests += !isPassed;
    failedOptionalTests += !isPassed && isOptional;
    testCasesReceived += (testIndex == 1);
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
        stream << "Test cases executed: " << testCasesReceived << "\n";
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

string TestLogger::getTestMessage(const string& groupDescription,
                                  const string& description,
                                  bool isOptional,
                                  bool isPassed,
                                  string failureMessage) {
    stringstream sBuffer;
    sBuffer << "[";
    modifyOutput(isPassed ? 32 : (isOptional ? 33 : 31), sBuffer);
    sBuffer << "X";
    modifyOutput(0, sBuffer);
    sBuffer << "] ";
    modifyOutput(90, sBuffer);
    sBuffer << groupDescription;
    modifyOutput(0, sBuffer);
    sBuffer << description;
    if (!isPassed) {
        sBuffer << "\n";
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
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
