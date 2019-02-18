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

void FeedbackExtension::destroy() {
    delete logger;
    delete pipe;
}

void FeedbackExtension::initLogging(ExtensionApi* api) {
    logger = new TestLogger(cout);

    api->afterTest([this](TestRun testRun) {
        logger->logTest(testRun);
    });

    api->beforeDestroy([this]() {
        logger->logFinalInformation();
    });

    api->beforeForceDestroy([this](const exception& error) {
        logger->logFatalError(error.what());
    });
}

void FeedbackExtension::initPipe(ExtensionApi* api, const string& pipeName) {
    pipe = openNamedPipeForWriting(pipeName);

    api->beforeGroup([this](GroupPtr group) {
        pipe->sendMessage(PipeMessageType::GROUP,
                          group->getParentGroup()->getIndex(),
                          group->getIndex(),
                          group->getDescription());
    });

    api->afterTest([this](TestRun testRun) {
        pipe->sendMessage(PipeMessageType::TEST,
                          testRun.getTest()->getGroup()->getIndex(),
                          testRun.getTest()->getIndex(),
                          testRun.getTest()->isOptional(),
                          testRun.getTest()->getDescription(),
                          testRun.isPassed(),
                          testRun.getFailure());
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
