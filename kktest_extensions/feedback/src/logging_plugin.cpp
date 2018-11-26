#include <iostream>

#include <EasyFlags.hpp>
#include <logging>

#include <kktest_plugin_api.hpp>

using namespace easyflags;
using namespace logging;
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
            allGroupsInfo[groupInfo.index] = groupInfo;
        });

        addAfterTestHook([this](const TestInfo& testInfo) {
            logger->logTest(testInfo.index,
                            getDescriptionPrefix(testInfo),
                            testInfo.description,
                            testInfo.optional,
                            testInfo.passed,
                            testInfo.failureMessage);
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
    string getDescriptionPrefix(const TestInfo& testInfo) {
        return testInfo.file + ":" + to_string(testInfo.line) + "::"
               + getRecursiveGroupDescription(testInfo.groupIndex);
    }

    string getRecursiveGroupDescription(int groupId) {
        auto groupInfoIterator = allGroupsInfo.find(groupId);
        if (groupInfoIterator == allGroupsInfo.end()) {
            return "";
        }
        GroupInfo groupInfo = groupInfoIterator->second;
        return getRecursiveGroupDescription(groupInfo.parentGroupIndex) + groupInfo.description + "::";
    }

    TestLogger* logger = nullptr;
    map<int, GroupInfo> allGroupsInfo;
};

LoggingPlugin loggingPlugin;

}
