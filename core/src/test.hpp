#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include <string>

#include "common/interproc/src/message.hpp"
#include "core/include/kktest_impl/config.hpp"
#include "core/src/info.hpp"

namespace kktest {

class Group;

struct TestExecutionInfo {
    enum MessageStatus: std::uint8_t {
        FINISHED_SUCCESSFULLY = 0,
        CONFIGURATION_ERROR = 1
    };

    static TestExecutionInfo fromError(const std::string& errorMessage);

    static TestExecutionInfo fromMessage(interproc::Message& message);

    TestExecutionInfo();

    TestExecutionInfo(double _executionTimeTicks,
                      bool _passed,
                      std::string _failureMessage);

    interproc::Message toMessage() const;

    double executionTimeTicks = -1.0;
    bool passed = true;
    std::string failureMessage = "";
};

class Test {
 public:
    Test(const TestConfig& _config, Group* _parentGroup, int _index);

    const TestConfig& getConfig() const;

    bool isExecuted() const;

    bool isFailed() const;

    TestInfo getTestInfo() const;

    Group* getGroup() const;

 private:
    void setExecuted(const TestExecutionInfo& _executionInfo);

    TestConfig config;

    Group* parentGroup;
    int index;

    bool executed = false;
    TestExecutionInfo executionInfo;

friend class Driver;
};

}

#endif
