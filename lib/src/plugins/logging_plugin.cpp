#include <unistd.h>

#include <iostream>
#include <sstream>
#include <set>

#include <EasyFlags.hpp>

#include <core/driver.hpp>

using namespace easyflags;
using namespace std;

AddArgument(int, flagQuiet)
    .ArgumentType("0|1 ")
    .Name("quiet")
    .Short("q")
    .Description("Disable STDOUT logging for this test run")
    .DefaultValue(0)
    .ImplicitValue(1);

namespace kktest {

class LoggingPlugin : public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override {
        return !flagQuiet;
    }

    void install() override {
        TestingDriver::addAfterTestHook([this](Test *test) {
            enqueueTestForLogging(test);
            numPassedTests += test->isPassed();
            numFailedTests += test->isFailed();
            numFailedOptionalTests += test->isFailed() && test->getConfig().optional;
        });
        TestingDriver::addBeforeDestroyHook([this]() {
            logFinalInformation();
        });
    }

private:
    std::string getTestMessage(Test* test) {
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

    bool isInTerminal() const {
        return isatty(fileno(stdout)) != 0;
    }

    void modifyOutput(const int& code, std::ostream& streamToChange) {
        if (!isInTerminal()) {
            return;
        }
        streamToChange << "\x1b[" << code << "m";
    }

    void enqueueTestForLogging(Test* test) {
        testsQueue.insert({test->getIndex(), getTestMessage(test)});
        while (!testsQueue.empty() && testsQueue.begin()->first == testsLogged + 1) {
            cout << testsQueue.begin()->second;
            testsQueue.erase(testsQueue.begin());
            testsLogged += 1;
        }
    }

    void logFinalInformation() {
        cout << "\n";
        cout << "Tests passed: ";
        modifyOutput(32, cout);
        cout << numPassedTests;
        modifyOutput(0, cout);
        cout << "\nTests failed: ";
        modifyOutput((numFailedTests == numFailedOptionalTests ?
                      (numFailedTests == 0 ? 32 : 33) : 31),
                     cout);
        cout << numFailedTests;
        modifyOutput(0, cout);
        if (numFailedOptionalTests != 0) {
            cout << " (";
            modifyOutput(33, cout);
            cout << numFailedOptionalTests;
            modifyOutput(0, cout);
            cout << " " << (numFailedOptionalTests == 1 ? "was" : "were") << " optional)";
        }
        cout << "\n";
    }

    int testsLogged = 0;
    set<pair<int, string>> testsQueue;

    int numPassedTests = 0;
    int numFailedTests = 0;
    int numFailedOptionalTests = 0;
};

LoggingPlugin loggingPlugin("kktest");

}
