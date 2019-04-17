#include "kktest/extensions/feedback/ext.hpp"

#include <iostream>
#include <memory>

using mcga::cli::ArgumentSpec;
using mcga::cli::FlagSpec;
using mcga::cli::Parser;
using namespace mcga::test;
using namespace mcga::proc;
using namespace std;

template<>
Message::BytesConsumer&
  Message::BytesConsumer::add(const vector<ExecutedTest::Info>& obj) {
    add(obj.size());
    for (const auto& info: obj) {
        add(info.timeTicks, info.passed, info.failure);
    }
    return *this;
}

namespace mcga::test::feedback {

enum PipeMessageType : uint8_t {
    TEST_STARTED = 0,
    TEST_DONE = 1,
    GROUP = 2,
    DONE = 3,
    WARNING = 4,
};

int FeedbackExtension::getReturnCode() {
    return failedAnyNonOptionalTests ? 1 : 0;
}

void FeedbackExtension::registerCommandLineArgs(Parser& parser) {
    quietFlag = parser.addFlag(
      FlagSpec("quiet")
        .setHelpGroup("Feedback")
        .setDescription("Disable STDOUT logging for this test run")
        .setShortName("q"));
    fileNameArgument = parser.addArgument(
      ArgumentSpec("pipe-to")
        .setHelpGroup("Feedback")
        .setDescription("A file or fifo with write access for piping the test "
                        "results as they become available.")
        .setDefaultValue(""));
}

void FeedbackExtension::init(HooksManager& api) {
    if (!quietFlag->getValue()) {
        initLogging(api);
    }
    if (!fileNameArgument->getValue().empty()) {
        initFileStream(api, fileNameArgument->getValue());
    }
    api.addHook<HooksManager::AFTER_TEST>([this](const ExecutedTest& test) {
        if (!test.isPassed() && !test.isOptional()) {
            failedAnyNonOptionalTests = true;
        }
    });
}

void FeedbackExtension::initLogging(HooksManager& api) {
    logger = make_unique<TestLogger>(cout);

    api.addHook<HooksManager::AFTER_TEST>(
      [this](const ExecutedTest& test) { logger->addTest(test); });

    api.addHook<HooksManager::BEFORE_DESTROY>(
      [this]() { logger->printFinalInformation(); });

    api.addHook<HooksManager::ON_WARNING>([this](const Warning& warning) {
        logger->printWarning(warning.message);
    });
}

void FeedbackExtension::initFileStream(HooksManager& api,
                                       const string& fileName) {
    fileWriter = unique_ptr<PipeWriter>(PipeWriter::OpenFile(fileName));

    api.addHook<HooksManager::BEFORE_GROUP>([this](GroupPtr group) {
        fileWriter->sendMessage(PipeMessageType::GROUP,
                                group->getParentGroup()->getId(),
                                group->getId(),
                                group->getDescription());
    });

    api.addHook<HooksManager::BEFORE_TEST>([this](const Test& test) {
        fileWriter->sendMessage(PipeMessageType::TEST_STARTED,
                                test.getId(),
                                test.getGroup()->getId(),
                                test.getDescription(),
                                test.isOptional(),
                                test.getNumAttempts(),
                                test.getNumRequiredPassedAttempts());
    });

    api.addHook<HooksManager::AFTER_TEST>([this](const ExecutedTest& test) {
        fileWriter->sendMessage(
          PipeMessageType::TEST_DONE, test.getId(), test.getExecutions());
    });

    api.addHook<HooksManager::BEFORE_DESTROY>(
      [this]() { fileWriter->sendMessage(PipeMessageType::DONE); });

    api.addHook<HooksManager::ON_WARNING>([this](const Warning& warning) {
        fileWriter->sendMessage(PipeMessageType::WARNING,
                                warning.message,
                                warning.groupId,
                                warning.testId);
    });
}

}  // namespace mcga::test::feedback
