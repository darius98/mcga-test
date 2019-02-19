#include "extensions/feedback/src/ext.hpp"

#include <iostream>

using namespace cppli;
using namespace kktest::interproc;
using namespace std;

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

void FeedbackExtension::init(ExtensionApi* api) {
    if (!quietFlag.get()) {
        initLogging(api);
    }
    if (!pipeNameArgument.get().empty()) {
        initPipe(api, pipeNameArgument.get());
    }
}

void FeedbackExtension::initLogging(ExtensionApi* api) {
    logger = unique_ptr<TestLogger>(new TestLogger(cout));

    api->afterTest([this](const ExecutedTest& test) {
        logger->logTest(test);
    });

    api->beforeDestroy([this]() {
        logger->logFinalInformation();
    });

    api->beforeForceDestroy([this](const exception& error) {
        logger->logFatalError(error.what());
    });
}

void FeedbackExtension::initPipe(ExtensionApi* api, const string& pipeName) {
    pipe = unique_ptr<PipeWriter>(openNamedPipeForWriting(pipeName));

    api->beforeGroup([this](GroupPtr group) {
        pipe->sendMessage(PipeMessageType::GROUP,
                          group->getParentGroup()->getIndex(),
                          group->getIndex(),
                          group->getDescription());
    });

    api->afterTest([this](const ExecutedTest& test) {
        pipe->sendMessage(PipeMessageType::TEST,
                          test.getGroup()->getIndex(),
                          test.getIndex(),
                          test.isOptional(),
                          test.getDescription(),
                          test.isPassed(),
                          test.getFailure());
    });

    api->beforeDestroy([this]() {
        pipe->sendMessage(PipeMessageType::DONE);
    });

    api->beforeForceDestroy([this](const exception& error) {
        pipe->sendMessage(PipeMessageType::ERROR, string(error.what()));
    });
}

}
}
