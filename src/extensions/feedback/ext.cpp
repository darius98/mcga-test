#include "ext.hpp"

#include <iostream>
#include <memory>

using mcga::cli::ArgumentSpec;
using mcga::cli::FlagSpec;
using mcga::cli::Parser;
using mcga::proc::createLocalClientSocket;
using mcga::proc::PipeWriter;
using mcga::test::Test;

namespace mcga::test {

enum PipeMessageType : uint8_t {
    GROUP_DISCOVERED = 0,
    TEST_DISCOVERED = 1,
    TEST_EXECUTION_START = 2,
    TEST_EXECUTION_FINISH = 3,
    DONE = 4,
    WARNING = 5,
};

void FeedbackExtension::registerCommandLineArgs(Parser* parser) {
    quietFlag = parser->add_flag(
      FlagSpec("quiet")
        .set_help_group("Feedback")
        .set_description("Disable STDOUT logging for this test run")
        .set_short_name("q"));
    printSkipped = parser->add_flag(
      FlagSpec("print-skipped")
        .set_help_group("Feedback")
        .set_description("Print the information for skipped tests."));
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
    if (socketPathArgument->appeared()) {
        initSocketStream(api, socketPathArgument->get_value());
    }
}

void FeedbackExtension::destroy() {
    if (logger != nullptr) {
        logger->printFinalInformation();
    }
    if (socketWriter != nullptr) {
        socketWriter->sendMessage(PipeMessageType::DONE);
    }
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

    api->addHook<ExtensionApi::BEFORE_TEST_EXECUTION>(
      [pipe](const Test& test, std::optional<Test::ExecutionInfo>&) {
          pipe->sendMessage(PipeMessageType::TEST_EXECUTION_START,
                            test.getId());
      });

    api->addHook<ExtensionApi::AFTER_TEST_EXECUTION>([pipe](const Test& test) {
        pipe->sendMessage(PipeMessageType::TEST_EXECUTION_FINISH,
                          test.getId(),
                          test.getExecutions().back().status,
                          test.getExecutions().back().timeTicks,
                          test.getExecutions().back().message,
                          test.getExecutions().back().context);
    });

    api->addHook<ExtensionApi::ON_WARNING>([pipe](const Warning& warning) {
        pipe->sendMessage(PipeMessageType::WARNING, warning);
    });
}

void FeedbackExtension::initLogging(ExtensionApi* api) {
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

void FeedbackExtension::initSocketStream(ExtensionApi* api,
                                         const std::string& socketPath) {
    socketWriter
      = std::unique_ptr<PipeWriter>(createLocalClientSocket(socketPath));
    addPipeHooks(socketWriter.get(), api);
}

int ExitCodeExtension::getExitCode() const {
    return exitCode;
}

void ExitCodeExtension::registerCommandLineArgs(cli::Parser* parser) {
    skipIsFail = parser->add_flag(
      FlagSpec("fail-on-skip")
        .set_help_group("Feedback")
        .set_description("Consider skipped tests as failed."));
    ignoreWarnings = parser->add_flag(
      FlagSpec("ignore-warnings")
        .set_help_group("Feedback")
        .set_description(
          "When this flag is set, a test run where all tests pass, but "
          "warnings were generated will exit with code 0 instead of 1."));
}

void ExitCodeExtension::init(ExtensionApi* api) {
    api->addHook<ExtensionApi::AFTER_TEST_EXECUTION>([this](const Test& test) {
        if (test.isFinished() && !test.isOptional()) {
            if (test.isFailed()
                || (test.isSkipped() && skipIsFail->get_value())) {
                exitCode = 1;
            }
        }
        if (test.isFinished() && test.isSkipped()) {
            skippedAnyTests = true;
        }
        if (test.isFinished() && test.isPassed()) {
            passedAnyTests = true;
        }
    });
    if (!ignoreWarnings->get_value()) {
        api->addHook<ExtensionApi::ON_WARNING>([this](const Warning& warning) {
            exitCode = 1;
        });
    }
}

void ExitCodeExtension::destroy() {
    if (skippedAnyTests && !passedAnyTests) {
        exitCode = 1;
    }
}

}  // namespace mcga::test
