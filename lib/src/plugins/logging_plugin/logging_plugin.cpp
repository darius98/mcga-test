#include <iostream>

#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "logger.hpp"

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

class LoggingPlugin: public Plugin {
public:
    bool isEnabled() const override {
        return !flagQuiet;
    }

    void install() override {
        logger = new Logger(cout);
        TestingDriver::addAfterTestHook([this](Test* test) {
            logger->enqueueTestForLogging(test);
            numPassedTests += test->isPassed();
            numFailedTests += test->isFailed();
            numFailedOptionalTests += test->isFailed() && test->getConfig().optional;
        });

        TestingDriver::addBeforeDestroyHook([this]() {
            logger->logFinalInformation(numPassedTests, numFailedTests, numFailedOptionalTests);
        });
    }

    void uninstall() override {
        delete logger;
    }

private:
    Logger* logger = nullptr;
    int numPassedTests = 0;
    int numFailedTests = 0;
    int numFailedOptionalTests = 0;
};

LoggingPlugin loggingPlugin;

}
