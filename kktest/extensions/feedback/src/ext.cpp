#include "kktest/extensions/feedback/src/ext.hpp"

#include <iostream>

using namespace cppli;
using namespace kktest::interproc;
using namespace std;

template<>
Message::BytesConsumer& Message::BytesConsumer::add(
        const vector<ExecutedTest::Info>& obj) {
    add(obj.size());
    for (const auto& info : obj) {
        add(info.timeTicks, info.passed, info.failure);
    }
    return *this;
}

namespace kktest {
namespace feedback {

enum PipeMessageType : uint8_t {
    TEST,
    GROUP,
    DONE,
    ERROR
};

void FeedbackExtension::registerCommandLineArgs(Parser& parser) {
    quietFlag = parser.addFlag(
        FlagSpec("quiet")
        .setHelpGroup("Feedback")
        .setDescription("Disable STDOUT logging for this test run")
        .setShortName("q"));
    pipeNameArgument = parser.addArgument(
        ArgumentSpec("pipe-to")
        .setHelpGroup("Feedback")
        .setDescription("A file or fifo with write access for piping the test "
                        "results as they become available."));
}

void FeedbackExtension::init(ExtensionApi& api) {
    if (!quietFlag.get()) {
        initLogging(api);
    }
    if (!pipeNameArgument.get().empty()) {
        initPipe(api, pipeNameArgument.get());
    }
}

void FeedbackExtension::initLogging(ExtensionApi& api) {
    logger = unique_ptr<TestLogger>(new TestLogger(cout));

    api.addHook<ExtensionApi::AFTER_TEST>([this](const ExecutedTest& test) {
        logger->addTest(test);
    });

    api.addHook<ExtensionApi::BEFORE_DESTROY>([this]() {
        logger->logFinalInformation();
    });

    api.addHook<ExtensionApi::BEFORE_FORCE_DESTROY>(
            [this](const exception& error) {
        logger->logFatalError(error.what());
    });
}

void FeedbackExtension::initPipe(ExtensionApi& api, const string& pipeName) {
    pipe = unique_ptr<PipeWriter>(openNamedPipeForWriting(pipeName));

    api.addHook<ExtensionApi::BEFORE_GROUP>([this](GroupPtr group) {
        pipe->sendMessage(PipeMessageType::GROUP,
                          group->getParentGroup()->getIndex(),
                          group->getIndex(),
                          group->getDescription());
    });

    api.addHook<ExtensionApi::AFTER_TEST>([this](const ExecutedTest& test) {
        pipe->sendMessage(PipeMessageType::TEST,
                          test.getGroup()->getIndex(),
                          test.getIndex(),
                          test.getDescription(),
                          test.isOptional(),
                          test.getNumAttempts(),
                          test.getNumRequiredPassedAttempts(),
                          test.getExecutions());
    });

    api.addHook<ExtensionApi::BEFORE_DESTROY>([this]() {
        pipe->sendMessage(PipeMessageType::DONE);
    });

    api.addHook<ExtensionApi::BEFORE_FORCE_DESTROY>(
            [this](const exception& error) {
        pipe->sendMessage(PipeMessageType::ERROR, string(error.what()));
    });
}

}
}
