#include <cstdio>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "logger.hpp"

using namespace std;

namespace kktest {

Logger::Logger(ostream& _stream): stream(_stream) {}

void Logger::enqueueTestForLogging(Test* test) {
    testsQueue.insert({test->getIndex(), getTestMessage(test)});
    while (!testsQueue.empty() && testsQueue.begin()->first == testsLogged + 1) {
        stream << testsQueue.begin()->second;
        testsQueue.erase(testsQueue.begin());
        testsLogged += 1;
    }
}

void Logger::logFinalInformation(int passedTests, int failedTests, int failedOptionalTests) {
    stream << "\n";
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

string Logger::getTestMessage(Test *test) {
    stringstream sBuffer;
    modifyOutput(90, sBuffer);
    sBuffer << test->getDescriptionPrefix();
    modifyOutput(0, sBuffer);
    sBuffer << test->getConfig().description;
    sBuffer << ": ";
    if (test->isFailed()) {
        modifyOutput(test->getConfig().optional ? 33: 31, sBuffer);
        sBuffer << "FAILED";
        modifyOutput(0, sBuffer);
        if (test->getConfig().optional) {
            sBuffer << " (optional)";
        }
        sBuffer << "\n";
        string failureMessage = test->getFailureMessage();
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        while ((pos = failureMessage.find('\n', pos)) != string::npos) {
            failureMessage.replace(pos, 1, "\n\t");
            pos += 2;
        }
        sBuffer << "\t" << failureMessage << "\n";
    } else {
        modifyOutput(32, sBuffer);
        sBuffer << "PASSED\n";
        modifyOutput(0, sBuffer);
    }
    return sBuffer.str();
}

bool Logger::isInTerminal() const {
    return stream.rdbuf() == cout.rdbuf() && isatty(fileno(stdout)) != 0;
}

void Logger::modifyOutput(const int& code, ostream& streamToChange) {
    if (!isInTerminal()) {
        return;
    }
    streamToChange << "\x1b[" << code << "m";
}

}
