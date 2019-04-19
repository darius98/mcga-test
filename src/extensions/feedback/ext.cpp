#include "ext.hpp"

#include <iostream>
#include <memory>

using mcga::cli::ArgumentSpec;
using mcga::cli::FlagSpec;
using mcga::cli::Parser;
using mcga::proc::Message;
using mcga::proc::PipeWriter;
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
      ArgumentSpec("pipe-to")
        .setHelpGroup("Feedback")
        .setDescription("A file or fifo with write access for piping the test "
                        "results as they become available.")
        .setDefaultValue(""));
}

void FeedbackExtension::init(HooksManager* api) {
    if (!quietFlag->getValue()) {
        initLogging(api);
    }
    if (!fileNameArgument->getValue().empty()) {
        initFileStream(api, fileNameArgument->getValue());
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

void FeedbackExtension::initLogging(HooksManager* api) {
    logger = make_unique<TestLogger>(cout);

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

    api->addHook<HooksManager::ON_GROUP_DISCOVERED>([this](GroupPtr group) {
        fileWriter->sendMessage(PipeMessageType::GROUP_DISCOVERED,
                                group->getParentGroup()->getId(),
                                group->getId(),
                                group->getDescription(),
                                group->isOptional());
    });

    api->addHook<HooksManager::ON_TEST_DISCOVERED>([this](const Test& test) {
        fileWriter->sendMessage(PipeMessageType::TEST_DISCOVERED,
                                test.getId(),
                                test.getGroup()->getId(),
                                test.getDescription(),
                                test.isOptional(),
                                test.getNumAttempts(),
                                test.getNumRequiredPassedAttempts());
    });

    api->addHook<HooksManager::ON_TEST_EXECUTION_START>(
      [this](const Test& test) {
          fileWriter->sendMessage(PipeMessageType::TEST_EXECUTION_START,
                                  test.getId());
      });

    api->addHook<HooksManager::ON_TEST_EXECUTION_FINISH>(
      [this](const Test& test) {
          fileWriter->sendMessage(PipeMessageType::TEST_EXECUTION_FINISH,
                                  test.getId(),
                                  test.getExecutions().back().timeTicks,
                                  test.getExecutions().back().passed,
                                  test.getExecutions().back().failure);
      });

    api->addHook<HooksManager::BEFORE_DESTROY>(
      [this]() { fileWriter->sendMessage(PipeMessageType::DONE); });

    api->addHook<HooksManager::ON_WARNING>([this](const Warning& warning) {
        fileWriter->sendMessage(PipeMessageType::WARNING,
                                warning.message,
                                warning.groupId,
                                warning.testId);
    });
}

}  // namespace mcga::test::feedback
