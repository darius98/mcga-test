#include <unistd.h>
#include <fcntl.h>

#include <EasyFlags.hpp>
#include <messaging>

#include <kktest_plugin_api.hpp>
#include <kktest_ext/feedback_impl/pipe_message_type.hpp>

using namespace easyflags;
using namespace kktest;
using namespace kktest::feedback;
using namespace messaging;
using namespace std;

AddArgument(string, argumentPipeFileName)
    .ArgumentType("FILE ")
    .Name("pipe_to")
    .Short("p")
    .Description("A file with write access for piping the test results as they become available.")
    .DefaultValue("");

class PipePlugin: public Plugin {
public:
    bool isEnabled() const override {
        return !argumentPipeFileName.empty();
    }

    void install() override {
        int pipeFD = open(argumentPipeFileName.c_str(), O_WRONLY | O_NONBLOCK);
        if (pipeFD < 0) {
            perror("open");
            exit(errno);
        }
        pipe = new OutputPipe(pipeFD);
        addBeforeGroupHook([this](const GroupInfo& groupInfo) {
            pipe->pipe(Message::build(PipeMessageType::GROUP,
                                      groupInfo.parentGroupIndex,
                                      groupInfo.index,
                                      groupInfo.line,
                                      groupInfo.file,
                                      groupInfo.description
            ));
        });
        addAfterTestHook([this](const TestInfo& testInfo) {
            pipe->pipe(Message::build(PipeMessageType::TEST,
                                      testInfo.groupIndex,
                                      testInfo.index,
                                      testInfo.line,
                                      testInfo.file,
                                      testInfo.optional,
                                      testInfo.description,
                                      testInfo.passed,
                                      testInfo.failureMessage
            ));
        });
        addBeforeDestroyHook([this]() {
            pipe->pipe(Message::build(PipeMessageType::DONE));
        });
        addBeforeForceDestroyHook([this](const std::exception& error) {
            pipe->pipe(Message::build(PipeMessageType::ERROR, string(error.what())));
        });
    }

    void uninstall() override {
        pipe->close();
        delete pipe;
    }

private:
    OutputPipe* pipe = nullptr;
};

volatile PipePlugin pipePlugin;
