#ifndef KKTEST_CORE_SRC_DRIVER_HPP_
#define KKTEST_CORE_SRC_DRIVER_HPP_

#include "core/src/errors.hpp"
#include "core/src/executor.hpp"
#include "core/src/extension_api.hpp"

namespace kktest {

class Driver {
 private:
    static Driver* instance;

 public:
    static Driver* getInstance();

    static Driver* init(
            const ExtensionApi& api, bool smooth, std::size_t numBoxes);

    Driver(ExtensionApi hooks, bool smooth, std::size_t numBoxes);

    int clean();

    void forceDestroy(const ConfigurationError& error);

    void beforeTestCase(const std::string& testCaseName);

    void afterTestCase();

    void addGroup(GroupConfig&& config, Executable func);

    void addTest(TestConfig&& config, Executable func);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

 private:
    void beforeTest(const Test& test);
    void afterTest(const ExecutedTest& test);
    void beforeGroup(GroupPtr group);
    void afterGroup(GroupPtr group);

    ExtensionApi extensionApi;

    std::unique_ptr<Executor> executor;
    std::vector<GroupPtr> groupStack = {};
    int currentTestIndex = 0;
    int groupIndex = 0;

    bool failedAnyNonOptionalTest = false;
};

}

#endif
