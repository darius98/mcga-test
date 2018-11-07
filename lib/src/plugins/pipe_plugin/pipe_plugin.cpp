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

bool PipePlugin::isEnabled() const {
    return argumentPipeFD != -1;
}

void PipePlugin::install() {
    pipe = new Pipe(argumentPipeFD);
    TestingDriver::addBeforeGroupHook([this](Group* group) {
        pipe->pipe(group);
    });
    TestingDriver::addAfterTestHook([this](Test* test) {
        pipe->pipe(test);
    });
}

void PipePlugin::uninstall() {
    delete pipe;
}

}
