#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "pipe_plugin.hpp"

using namespace easyflags;
using namespace std;

AddArgument(int, argumentPipeFD)
    .ArgumentType("FILE DESCRIPTOR ")
    .Name("pipe_to")
    .Short("p")
    .Description("A file descriptor with write access for piping the test"
                 "results as soon as they are available.")
    .DefaultValue(-1);

namespace kktest {

Plugin* pipePlugin = new PipePlugin("kktest");

bool PipePlugin::isEnabled() const {
    return argumentPipeFD != -1;
}

void PipePlugin::install() {
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

void PipePlugin::uninstall() {
    delete pipe;
}

}
