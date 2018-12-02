#include <unistd.h>
#include <fcntl.h>

#include <iostream>

#include <kktest_extension_api.hpp>
#include <kktest_ext/feedback_impl/ext.hpp>
#include <kktest_ext/feedback_impl/pipe_message_type.hpp>

using messaging::Message;
using messaging::OutputPipe;
using std::cout;
using std::exception;

namespace kktest {
namespace feedback {

void FeedbackExtension::registerCommandLineArguments(ArgumentsApi* argumentsApi) {
    quietFlag = argumentsApi->addFlag("quiet", "Disable STDOUT logging for this test run", "q");
    pipeNameArgument = argumentsApi->addArgument(
            "pipe_to",
            "A file with write access for piping the test results as they become available.");
}

void FeedbackExtension::init(ExtensionApi* api) {
    if (!quietFlag->get()) {
        initLogging(api);
    }
    if (!pipeNameArgument->get().empty()) {
        initPipe(api, pipeNameArgument->get());
    }
}

void FeedbackExtension::destroy() {
    delete logger;
    if (pipe) {
        pipe->close();
    }
    delete pipe;
}

void FeedbackExtension::initLogging(ExtensionApi* api) {
    logger = new TestLogger(cout);

    api->addBeforeGroupHook([this](const GroupInfo& groupInfo) {
        logger->addGroupInfo(groupInfo);
    });

    api->addAfterTestHook([this](const TestInfo& testInfo) {
        logger->logTest(testInfo);
    });

    api->addBeforeDestroyHook([this]() {
        logger->logFinalInformation();
    });

    api->addBeforeForceDestroyHook([this](const exception& error) {
        logger->logFatalError(error.what());
    });
}

void FeedbackExtension::initPipe(ExtensionApi* api, const String& pipeName) {
    int pipeFD = open(pipeName.c_str(), O_WRONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        perror("open");
        exit(errno);
    }
    pipe = new OutputPipe(pipeFD);

    api->addBeforeGroupHook([this](const GroupInfo& groupInfo) {
        pipe->pipe(Message::build(PipeMessageType::GROUP,
                                  groupInfo.parentGroupIndex,
                                  groupInfo.index,
                                  groupInfo.description));
    });

    api->addAfterTestHook([this](const TestInfo& testInfo) {
        pipe->pipe(Message::build(PipeMessageType::TEST,
                                  testInfo.groupIndex,
                                  testInfo.index,
                                  testInfo.optional,
                                  testInfo.description,
                                  testInfo.passed,
                                  testInfo.failureMessage));
    });

    api->addBeforeDestroyHook([this]() {
        pipe->pipe(Message::build(PipeMessageType::DONE));
    });

    api->addBeforeForceDestroyHook([this](const exception& error) {
        pipe->pipe(Message::build(PipeMessageType::ERROR, String(error.what())));
    });
}

}  // namespace feedback
}  // namespace kktest