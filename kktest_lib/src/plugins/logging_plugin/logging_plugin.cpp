#include <iostream>

#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "logging_plugin.hpp"

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

bool LoggingPlugin::isEnabled() const {
    return !flagQuiet;
}

void LoggingPlugin::install() {
    logger = new Logger(cout);
    TestingDriver::addAfterTestHook([this](Test* test) {
        logger->enqueueTestForLogging(test);
    });
}

void LoggingPlugin::uninstall() {
    delete logger;
}

}
