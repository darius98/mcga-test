#include "ext.hpp"

#include <iostream>
#include <memory>

using mcga::cli::ArgumentSpec;
using mcga::cli::FlagSpec;
using mcga::cli::Parser;
using mcga::proc::PipeWriter;
using mcga::proc::createLocalClientSocket;
using mcga::test::Test;
using std::cout;
using std::make_unique;
using std::string;
using std::unique_ptr;

namespace mcga::test::feedback {

enum PipeMessageType : uint8_t {
    GROUP_DISCOVERED = 0,
    TEST_DISCOVERED = 1,
    TEST_EXECUTION_START = 2,
    TEST_EXECUTION_FINISH = 3,
    DONE = 4,
    WARNING = 5,
};

int FeedbackExtension::getReturnCode() {
    return exitCode;
}

void FeedbackExtension::registerCommandLineArgs(Parser* parser) {
    quietFlag = parser->addFlag(
      FlagSpec("quiet")
        .setHelpGroup("Feedback")
        .setDescription("Disable STDOUT logging for this test run")
        .setShortName("q"));
    fileNameArgument = parser->addArgument(
      ArgumentSpec("stream-to-file")
        .setHelpGroup("Feedback")
        .setDescription("A file with write access for piping the test "
                        "results as they become available.")
        .setDefaultValue(""));
    socketPathArgument = parser->addArgument(
      ArgumentSpec("stream-to-socket")
        .setHelpGroup("Feedback")
        .setDescription("A UNIX socket with write access for piping the test "
                        "results as they become available.")
        .setDefaultValue(""));
    noLiveLogging = parser->addFlag(
      FlagSpec("no-live-logging")
        .setHelpGroup("Feedback")
        .setDescription("Disable logging volatile messages "
                        "(that are then removed with '\\r' modifiers."));
}

void FeedbackExtension::init(HooksManager* api) {
    if (!quietFlag->getValue()) {
        initLogging(api);
    }
    if (fileNameArgument->appeared()) {
        initFileStream(api, fileNameArgument->getValue());
    }
    if (socketPathArgument->appeared()) {
        initSocketStream(api, socketPathArgument->getValue());
    }
    api->addHook<HooksManager::ON_TEST_EXECUTION_FINISH>(
      [this](const Test& test) {
          if (test.isExecuted() && !test.isPassed() && !test.isOptional()) {
              exitCode = 1;
          }
      });
    api->addHook<HooksManager::ON_WARNING>(
      [this](const Warning& warning) { exitCode = 1; });
}

void FeedbackExtension::addPipeHooks(PipeWriter* pipe, HooksManager* api) {
    api->addHook<HooksManager::ON_GROUP_DISCOVERED>([pipe](GroupPtr group) {
        pipe->sendMessage(PipeMessageType::GROUP_DISCOVERED,
                          group->getParentGroup()->getId(),
                          group->getId(),
                          group->getDescription(),
                          group->isOptional());
    });

    api->addHook<HooksManager::ON_TEST_DISCOVERED>([pipe](const Test& test) {
        pipe->sendMessage(PipeMessageType::TEST_DISCOVERED,
                          test.getId(),
                          test.getGroup()->getId(),
                          test.getDescription(),
                          test.isOptional(),
                          test.getNumAttempts(),
                          test.getNumRequiredPassedAttempts());
    });

    api->addHook<HooksManager::ON_TEST_EXECUTION_START>([pipe](
                                                          const Test& test) {
        pipe->sendMessage(PipeMessageType::TEST_EXECUTION_START, test.getId());
    });

    api->addHook<HooksManager::ON_TEST_EXECUTION_FINISH>(
      [pipe](const Test& test) {
          pipe->sendMessage(PipeMessageType::TEST_EXECUTION_FINISH,
                            test.getId(),
                            test.getExecutions().back().timeTicks,
                            test.getExecutions().back().passed,
                            test.getExecutions().back().failure);
      });

    api->addHook<HooksManager::BEFORE_DESTROY>(
      [pipe]() { pipe->sendMessage(PipeMessageType::DONE); });

    api->addHook<HooksManager::ON_WARNING>([pipe](const Warning& warning) {
        pipe->sendMessage(PipeMessageType::WARNING,
                          warning.message,
                          warning.groupId,
                          warning.testId);
    });
}

void FeedbackExtension::initLogging(HooksManager* api) {
    logger = make_unique<TestLogger>(cout, !noLiveLogging->getValue());

    api->addHook<HooksManager::ON_TEST_EXECUTION_START>(
      [this](const Test& test) { logger->onTestExecutionStart(test); });

    api->addHook<HooksManager::ON_TEST_EXECUTION_FINISH>(
      [this](const Test& test) { logger->onTestExecutionFinish(test); });

    api->addHook<HooksManager::BEFORE_DESTROY>(
      [this]() { logger->printFinalInformation(); });

    api->addHook<HooksManager::ON_WARNING>([this](const Warning& warning) {
        logger->printWarning(warning.message);
    });
}

void FeedbackExtension::initFileStream(HooksManager* api,
                                       const string& fileName) {
    fileWriter = unique_ptr<PipeWriter>(PipeWriter::OpenFile(fileName));
    addPipeHooks(fileWriter.get(), api);
}

void FeedbackExtension::initSocketStream(HooksManager* api,
                                         const string& socketPath) {
    socketWriter = unique_ptr<PipeWriter>(createLocalClientSocket(socketPath));
    addPipeHooks(socketWriter.get(), api);
}

}  // namespace mcga::test::feedback
