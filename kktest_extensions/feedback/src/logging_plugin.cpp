#include <iostream>

#include <EasyFlags.hpp>

#include <kktest_ext/feedback_impl/logging_plugin.hpp>
#include <kktest_plugin_api.hpp>

using namespace easyflags;
using namespace kktest;
using namespace std;

namespace kktest {
namespace feedback {

void LoggingPlugin::install() {
    logger = new TestLogger(cout);

    addBeforeGroupHook([this](const GroupInfo& groupInfo) {
        logger->addGroupInfo(groupInfo);
    });

    addAfterTestHook([this](const TestInfo& testInfo) {
        logger->logTest(testInfo);
    });

    addBeforeDestroyHook([this]() {
        logger->logFinalInformation();
    });

    addBeforeForceDestroyHook([this](const exception& error) {
        logger->logFatalError(error.what());
    });
}

void LoggingPlugin::uninstall() {
    delete logger;
}

}
}
