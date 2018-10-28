#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "pipe_module.hpp"

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

PipeModule::PipeModule() {
    if (isEnabled()) {
        pipe = new Pipe(argumentPipeFD);
    }
}

PipeModule::~PipeModule() {
    delete pipe;
}

bool PipeModule::isEnabled() const {
    return argumentPipeFD != -1;
}

void PipeModule::install() {
    TestingDriver::addBeforeGroupHook([this](Group* group) {
        pipe->pipeGroup(group);
    });
    TestingDriver::addAfterTestHook([this](Test* test) {
        pipe->pipeTest(test);
    });
}

}
