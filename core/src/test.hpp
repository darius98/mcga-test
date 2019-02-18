#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include <memory>
#include <string>

#include "common/interproc/src/message.hpp"
#include "core/include/kktest_impl/config.hpp"
#include "core/src/info.hpp"

namespace kktest {

class Group;

class ExecutionInfo {
 public:
    enum MessageStatus: std::uint8_t {
        FINISHED_SUCCESSFULLY = 0,
        CONFIGURATION_ERROR = 1
    };

    explicit ExecutionInfo(const std::string& errorMessage);
    explicit ExecutionInfo(interproc::Message& message);
    ExecutionInfo(double _timeTicks, bool _passed, std::string _failure);

    interproc::Message toMessage() const;

 private:
    double timeTicks;
    bool passed;
    std::string failure;

friend class Test;
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
    void setExecuted(const ExecutionInfo& info);

    TestConfig config;
    Group* parentGroup;
    int index;
    std::unique_ptr<ExecutionInfo> executionInfo = nullptr;

friend class Driver;
};

}

#endif
