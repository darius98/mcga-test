#include "ext.hpp"

#include <iostream>
#include <memory>

using mcga::cli::ArgumentSpec;
using mcga::cli::FlagSpec;
using mcga::cli::Parser;
using mcga::proc::createLocalClientSocket;
using mcga::proc::PipeWriter;
using mcga::test::Test;

namespace mcga::test::feedback {

enum PipeMessageType : uint8_t {
    GROUP_DISCOVERED = 0,
    TEST_DISCOVERED = 1,
    TEST_EXECUTION_START = 2,
    TEST_EXECUTION_FINISH = 3,
    DONE = 4,
    WARNING = 5,
};

int FeedbackExtension::getReturnCode() const {
    return exitCode;
}

void FeedbackExtension::registerCommandLineArgs(Parser* parser) {
    quietFlag = parser->add_flag(
      FlagSpec("quiet")
        .set_help_group("Feedback")
        .set_description("Disable STDOUT logging for this test run")
        .set_short_name("q"));
    fileNameArgument = parser->add_argument(
      ArgumentSpec("stream-to-file")
        .set_help_group("Feedback")
        .set_description("A file with write access for piping the test "
                         "results as they become available.")
        .set_default_value(""));
    socketPathArgument = parser->add_argument(
      ArgumentSpec("stream-to-socket")
        .set_help_group("Feedback")
        .set_description("A UNIX socket with write access for piping the test "
                         "results as they become available.")
        .set_default_value(""));
    noLiveLogging = parser->add_flag(
      FlagSpec("no-live-logging")
        .set_help_group("Feedback")
        .set_description("Disable logging volatile messages "
                         "(that are then removed with '\\r' modifiers."));
}

void FeedbackExtension::init(ExtensionApi* api) {
    if (!quietFlag->get_value()) {
        initLogging(api);
    }
    if (fileNameArgument->appeared()) {
        initFileStream(api, fileNameArgument->get_value());
    }
    if (socketPathArgument->appeared()) {
        initSocketStream(api, socketPathArgument->get_value());
    }
    api->addHook<ExtensionApi::AFTER_TEST_EXECUTION>([this](const Test& test) {
        if (test.isFinished() && !test.isPassed() && !test.isOptional()) {
            exitCode = 1;
        }
    });
    api->addHook<ExtensionApi::ON_WARNING>(
      [this](const Warning& warning) { exitCode = 1; });
}

void FeedbackExtension::addPipeHooks(PipeWriter* pipe, ExtensionApi* api) {
    api->addHook<ExtensionApi::ON_GROUP_DISCOVERED>([pipe](GroupPtr group) {
        pipe->sendMessage(PipeMessageType::GROUP_DISCOVERED,
                          group->getParentGroup()->getId(),
                          group->getId(),
                          group->getDescription(),
                          group->isOptional());
    });

    api->addHook<ExtensionApi::ON_TEST_DISCOVERED>([pipe](const Test& test) {
        pipe->sendMessage(PipeMessageType::TEST_DISCOVERED,
                          test.getId(),
                          test.getGroup()->getId(),
                          test.getDescription(),
                          test.isOptional(),
                          test.getNumAttempts(),
                          test.getNumRequiredPassedAttempts());
    });

    api->addHook<ExtensionApi::BEFORE_TEST_EXECUTION>([pipe](const Test& test) {
        pipe->sendMessage(PipeMessageType::TEST_EXECUTION_START, test.getId());
    });

    api->addHook<ExtensionApi::AFTER_TEST_EXECUTION>([pipe](const Test& test) {
        pipe->sendMessage(PipeMessageType::TEST_EXECUTION_FINISH,
                          test.getId(),
                          test.getExecutions().back().status,
                          test.getExecutions().back().timeTicks,
                          test.getExecutions().back().message,
                          test.getExecutions().back().context);
    });

    api->addHook<ExtensionApi::BEFORE_DESTROY>(
      [pipe]() { pipe->sendMessage(PipeMessageType::DONE); });

    api->addHook<ExtensionApi::ON_WARNING>([pipe](const Warning& warning) {
        pipe->sendMessage(PipeMessageType::WARNING, warning);
    });
}

void FeedbackExtension::initLogging(ExtensionApi* api) {
    logger = make_unique<TestLogger>(std::cout, !noLiveLogging->get_value());

    api->addHook<ExtensionApi::BEFORE_TEST_EXECUTION>(
      [this](const Test& test) { logger->onTestExecutionStart(test); });

    api->addHook<ExtensionApi::AFTER_TEST_EXECUTION>(
      [this](const Test& test) { logger->onTestExecutionFinish(test); });

    api->addHook<ExtensionApi::BEFORE_DESTROY>(
      [this]() { logger->printFinalInformation(); });

    api->addHook<ExtensionApi::ON_WARNING>(
      [this](const Warning& warning) { logger->printWarning(warning); });
}

void FeedbackExtension::initFileStream(ExtensionApi* api,
                                       const std::string& fileName) {
    fileWriter = std::unique_ptr<PipeWriter>(PipeWriter::OpenFile(fileName));
    addPipeHooks(fileWriter.get(), api);
}

void FeedbackExtension::initSocketStream(ExtensionApi* api,
                                         const std::string& socketPath) {
    socketWriter
      = std::unique_ptr<PipeWriter>(createLocalClientSocket(socketPath));
    addPipeHooks(socketWriter.get(), api);
}

}  // namespace mcga::test::feedback
