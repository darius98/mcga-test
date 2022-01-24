#include "stdout_logging_extension.hpp"

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

void StdoutLoggingExtension::init(ExtensionApi* api) {
    if (!quietFlag->get_value()) {
        initLogging(api);
    }
}

void StdoutLoggingExtension::destroy() {
    if (logger != nullptr) {
        logger->printFinalInformation();
    }
}

void StdoutLoggingExtension::initLogging(ExtensionApi* api) {
    logger = make_unique<TestLogger>(
      std::cout, !noLiveLogging->get_value(), printSkipped->get_value());

    api->addHook<ExtensionApi::BEFORE_TEST_EXECUTION>(
      [this](const Test& test, std::optional<Test::ExecutionInfo>&) {
          logger->onTestExecutionStart(test);
      });

    api->addHook<ExtensionApi::AFTER_TEST_EXECUTION>([this](const Test& test) {
        logger->onTestExecutionFinish(test);
    });

    api->addHook<ExtensionApi::ON_WARNING>([this](const Warning& warning) {
        logger->printWarning(warning);
    });
}

}  // namespace mcga::test
