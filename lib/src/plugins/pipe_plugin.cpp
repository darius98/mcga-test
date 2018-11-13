#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include <pipe.hpp>

using namespace easyflags;
using namespace messaging;
using namespace std;

AddArgument(int, argumentPipeFD)
    .ArgumentType("FILE DESCRIPTOR ")
    .Name("pipe_to")
    .Short("p")
    .Description("A file descriptor with write access for piping the test"
                 "results as soon as they are available.")
    .DefaultValue(-1);

namespace kktest {

class PipePlugin: public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override {
        return argumentPipeFD != -1;
    }

    void install() override {
        pipe = new OutputPipe(argumentPipeFD);
        TestingDriver::addBeforeGroupHook([this](Group* group) {
            pipe->pipe(Message::build([group](BytesConsumer& consumer) {
                consumer
                        << group->getParentGroupIndex()
                        << group->getIndex()
                        << group->getConfig().line
                        << group->getConfig().file
                        << group->getConfig().description;
            }));
        });
        TestingDriver::addAfterTestHook([this](Test* test) {
            pipe->pipe(Message::build([test](BytesConsumer& consumer) {
                consumer
                        << test->getGroupIndex()
                        << test->getIndex()
                        << test->getConfig().line
                        << test->getConfig().file
                        << test->getConfig().optional
                        << test->getConfig().description
                        << test->isPassed()
                        << test->getFailureMessage();
            }));
        });
    }

    void uninstall() override {
        delete pipe;
    }

private:
    OutputPipe* pipe = nullptr;
};

PipePlugin pipePlugin("kktest");

}
