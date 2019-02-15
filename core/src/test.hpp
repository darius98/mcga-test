#ifndef KKTEST_CORE_TEST_HPP_
#define KKTEST_CORE_TEST_HPP_

#include <extension_api/info.hpp>
#include <kktest_common/interproc.hpp>
#include <kktest_impl/config.hpp>

namespace kktest {

class Group;

struct TestExecutionInfo {
    enum MessageStatus { FINISHED_SUCCESSFULLY, CONFIGURATION_ERROR };

    double executionTimeTicks = -1.0;
    bool passed = true;
    String failureMessage = "";

    static interproc::Message toErrorMessage(const String& errorMessage);

    static TestExecutionInfo fromMessage(const interproc::Message& message);

    interproc::Message toMessage() const;
};

class Test {
 public:
    Test(const TestConfig& _config, Group* _parentGroup, int _index);

    const TestConfig& getConfig() const;

    bool isExecuted() const;

    bool isFailed() const;

    TestInfo getTestInfo() const;

    Group* getGroup() const;

    void setExecuted(const TestExecutionInfo& _executionInfo);

 private:
    TestConfig config;

    Group* parentGroup;
    int index;

    bool executed = false;
    TestExecutionInfo executionInfo;
};

}

#endif
