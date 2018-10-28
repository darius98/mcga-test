#include <iostream>

#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "logging_module.hpp"

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

LoggingModule::LoggingModule() {
    if (isEnabled()) {
        logger = new Logger(cout);
    }
}

LoggingModule::~LoggingModule() {
    delete logger;
}

bool LoggingModule::isEnabled() const {
    return !flagQuiet;
}

void LoggingModule::install() {
    TestingDriver::addAfterTestHook([this](Test* test) {
        logger->enqueueTestForLogging(test);
    });
}

}
