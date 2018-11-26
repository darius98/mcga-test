#include <iostream>

#include <EasyFlags.hpp>

#include <kktest_ext/feedback_impl/test_logger.hpp>
#include <kktest_plugin_api.hpp>

using namespace easyflags;
using namespace std;

AddArgument(int, flagQuiet)
    .ArgumentType("0|1 ")
    .Name("quiet")
    .Short("q")
    .Description("Disable STDOUT logging for this test run")
    .DefaultValue(0)
    .ImplicitValue(1);

namespace kktest {

class LoggingPlugin: public Plugin {
public:
    bool isEnabled() const override {
        return !flagQuiet;
    }

    void install() override {
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

    void uninstall() override {
        delete logger;
    }

private:
    TestLogger* logger = nullptr;
};

LoggingPlugin loggingPlugin;

}
