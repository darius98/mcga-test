#include <EasyFlags.hpp>

#include <kktest_ext/feedback_impl/ext.hpp>
#include <kktest_ext/feedback_impl/logging_plugin.hpp>
#include <kktest_ext/feedback_impl/pipe_plugin.hpp>
#include <kktest_plugin_api.hpp>

using namespace easyflags;
using namespace kktest;
using namespace std;

AddArgument(int, flagQuiet)
    .ArgumentType("0|1 ")
    .Name("quiet")
    .Short("q")
    .Description("Disable STDOUT logging for this test run")
    .DefaultValue(0)
    .ImplicitValue(1);

AddArgument(string, argumentPipeFileName)
        .ArgumentType("FILE ")
        .Name("pipe_to")
        .Short("p")
        .Description("A file with write access for piping the test results as they become available.")
        .DefaultValue("");

namespace kktest {
namespace feedback {

void FeedbackExtensionManager::init(const function<void(Plugin*)>& registerPlugin) {
    if (!flagQuiet) {
        loggingPlugin = new LoggingPlugin();
        registerPlugin(loggingPlugin);
    }
    if (!argumentPipeFileName.empty()) {
        pipePlugin = new PipePlugin(argumentPipeFileName);
        registerPlugin(pipePlugin);
    }
}

void FeedbackExtensionManager::destroy() {
    delete loggingPlugin;
    delete pipePlugin;
}

}
}