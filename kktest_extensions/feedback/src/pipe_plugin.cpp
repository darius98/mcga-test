#include <unistd.h>
#include <fcntl.h>

#include <EasyFlags.hpp>
#include <messaging>

#include <kktest_plugin_api.hpp>

using namespace easyflags;
using namespace messaging;
using namespace std;

AddArgument(string, argumentPipeFileName)
    .ArgumentType("FILE ")
    .Name("pipe_to")
    .Short("p")
    .Description("A file with write access for piping the test results as they become available.")
    .DefaultValue("");

namespace kktest {

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
            pipe->pipe(Message::build(0,
                                      groupInfo.parentGroupIndex,
                                      groupInfo.index,
                                      groupInfo.line,
                                      groupInfo.file,
                                      groupInfo.description
            ));
        });
        addAfterTestHook([this](const TestInfo& testInfo) {
            pipe->pipe(Message::build(1,
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
            pipe->pipe(Message::build(2));
        });
        addBeforeForceDestroyHook([this](const std::exception& error) {
            pipe->pipe(Message::build(3, string(error.what())));
        });
    }

    void uninstall() override {
        pipe->close();
        delete pipe;
    }

private:
    OutputPipe* pipe = nullptr;
};

PipePlugin pipePlugin;

}
