#include <iostream>

#include <EasyFlags.hpp>
#include <logging>

#include <core/driver.hpp>

using namespace easyflags;
using namespace logging;
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
        logger = new TestLogger(cout);
        TestingDriver::addAfterTestHook([this](const Test& test) {
            logger->logTest(test.getIndex(),
                            test.getDescriptionPrefix(),
                            test.getConfig().description,
                            test.getConfig().optional,
                            test.isPassed(),
                            test.getFailureMessage());
        });

        TestingDriver::addBeforeDestroyHook([this]() {
            logger->logFinalInformation();
        });

        TestingDriver::addBeforeForceDestroyHook([this](const ConfigurationError& error) {
            logger->logFatalError(error.what());
        });
    }

    void uninstall() override {
        delete logger;
    }

private:
    TestLogger* logger = nullptr;
};

LoggingPlugin loggingPlugin;

}
