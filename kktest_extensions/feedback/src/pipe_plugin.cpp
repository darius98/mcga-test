#include <unistd.h>
#include <fcntl.h>

#include <EasyFlags.hpp>
#include <messaging>

#include <kktest_plugin_api.hpp>
#include <kktest_ext/feedback_impl/pipe_message_type.hpp>
#include <kktest_ext/feedback_impl/pipe_plugin.hpp>

using namespace easyflags;
using namespace messaging;
using namespace std;

namespace kktest {
namespace feedback {

PipePlugin::PipePlugin(const string& _pipeName): pipeName(_pipeName) {}

void PipePlugin::install() {
    int pipeFD = open(pipeName.c_str(), O_WRONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        perror("open");
        exit(errno);
    }
    pipe = new OutputPipe(pipeFD);
    addBeforeGroupHook([this](const GroupInfo& groupInfo) {
        pipe->pipe(Message::build(PipeMessageType::GROUP,
                                  groupInfo.parentGroupIndex,
                                  groupInfo.index,
                                  groupInfo.description
        ));
    });
    addAfterTestHook([this](const TestInfo& testInfo) {
        pipe->pipe(Message::build(PipeMessageType::TEST,
                                  testInfo.groupIndex,
                                  testInfo.index,
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

void PipePlugin::uninstall() {
    pipe->close();
    delete pipe;
}

}
}