#include <iostream>

#include <EasyFlags.hpp>

#include <kktest_ext/feedback_impl/logging_plugin.hpp>
#include <kktest_plugin_api.hpp>

using namespace easyflags;
using namespace kktest;
using namespace std;

namespace kktest {
namespace feedback {

void LoggingPlugin::install(PluginApi* api) {
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

void LoggingPlugin::uninstall() {
    delete logger;
}

}
}
