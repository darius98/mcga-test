#include <unistd.h>
#include <fcntl.h>

#include <EasyFlags.hpp>
#include <messaging>

#include <core/driver.hpp>

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
        TestingDriver::addBeforeGroupHook([this](const Group& group) {
            pipe->pipe(Message::build(0,
                                      group.getParentGroupIndex(),
                                      group.getIndex(),
                                      group.getConfig().line,
                                      group.getConfig().file,
                                      group.getConfig().description
            ));
        });
        TestingDriver::addAfterTestHook([this](const Test& test) {
            pipe->pipe(Message::build(1,
                                      test.getGroupIndex(),
                                      test.getIndex(),
                                      test.getConfig().line,
                                      test.getConfig().file,
                                      test.getConfig().optional,
                                      test.getConfig().description,
                                      test.isPassed(),
                                      test.getFailureMessage()
            ));
        });
        TestingDriver::addBeforeDestroyHook([this]() {
            pipe->pipe(Message::build(2));
        });
        TestingDriver::addBeforeForceDestroyHook([this](const ConfigurationError& error) {
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
