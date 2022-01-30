#include "ext.hpp"

#include <iostream>
#include <memory>

namespace mcga::test {

void StdoutLoggingExtension::registerCommandLineArgs(cli::Parser* parser) {
    quietFlag = parser->add_flag(
      cli::FlagSpec("quiet")
        .set_help_group("Feedback")
        .set_description("Disable STDOUT logging for this test run")
        .set_short_name("q"));
    printSkipped = parser->add_flag(
      cli::FlagSpec("print-skipped")
        .set_help_group("Feedback")
        .set_description("Print the information for skipped tests."));
    noLiveLogging = parser->add_flag(
      cli::FlagSpec("no-live-logging")
        .set_help_group("Feedback")
        .set_description("Disable logging volatile messages "
                         "(that are then removed with '\\r' modifiers."));
}

void StdoutLoggingExtension::init() {
    if (!quietFlag->get_value()) {
        logger = make_unique<TestLogger>(
          std::cout, !noLiveLogging->get_value(), printSkipped->get_value());
    }
}

void StdoutLoggingExtension::beforeTestExecution(const Test& test) {
    if (logger != nullptr) {
        logger->onTestExecutionStart(test);
    }
}

void StdoutLoggingExtension::afterTestExecution(const Test& test) {
    if (logger != nullptr) {
        logger->onTestExecutionFinish(test);
    }
}

void StdoutLoggingExtension::onWarning(const Warning& warning) {
    if (logger != nullptr) {
        logger->printWarning(warning);
    }
}

void StdoutLoggingExtension::destroy() {
    if (logger != nullptr) {
        logger->printFinalInformation();
    }
}

}  // namespace mcga::test
