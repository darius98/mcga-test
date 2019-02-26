#ifndef KKTEST_CORE_SRC_DRIVER_HPP_
#define KKTEST_CORE_SRC_DRIVER_HPP_

#include "kktest/core/src/errors.hpp"
#include "kktest/core/src/executor.hpp"
#include "kktest/core/src/extension_api.hpp"

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

    void addGroup(GroupConfig&& config, const Executable& func);

    void addTest(TestConfig&& config, const Executable& func);

    void addSetUp(const Executable& func);

    void addTearDown(const Executable& func);

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
